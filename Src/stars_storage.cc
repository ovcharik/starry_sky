#include "stars_storage.h"

#include "defines.h"
#include "event_repository.h"

#include <ctime>
#include <cmath>
#include <cstdlib>

#include <gtkmm.h>

StarsStorage::StarsStorage()
  : m_star_count(DEFAULT_STAR_COUNT),
    m_star_time_of_live(DEFAULT_STAR_TIME_OF_LIFE),
    m_big_percent(DEFAULT_STAR_BIG),
    m_middle_percent(DEFAULT_STAR_MIDDLE),
    m_small_percent(DEFAULT_STAR_SMALL)
{
  on_count_changed();
  
  // bind events
  EventRepository::get_event<Gtk::Adjustment*>(EventTypes::LP_STAR_COUNT)
    ->add_handler([this](Gtk::Adjustment* obj){
      this->m_star_count = obj->get_value();
      this->on_count_changed();
    });
  
  EventRepository::get_event<Gtk::Adjustment*>(EventTypes::LP_STAR_TIME_OF_LIVE)
    ->add_handler([this](Gtk::Adjustment* obj){
      this->m_star_time_of_live = obj->get_value();
    });
  
  EventRepository::get_event<int>(EventTypes::SS_TIMER)
    ->add_handler([this](int value){
      this->remove_oldest();
    });
  
  EventRepository::get_event<Gtk::Adjustment*>(EventTypes::LP_PERCENT_BIG)
    ->add_handler([this](Gtk::Adjustment* obj){
      this->m_big_percent = obj->get_value();
    });
  EventRepository::get_event<Gtk::Adjustment*>(EventTypes::LP_PERCENT_MIDDLE)
    ->add_handler([this](Gtk::Adjustment* obj){
      this->m_middle_percent = obj->get_value();
    });
  EventRepository::get_event<Gtk::Adjustment*>(EventTypes::LP_PERCENT_SMALL)
    ->add_handler([this](Gtk::Adjustment* obj){
      this->m_small_percent = obj->get_value();
    });
}

StarsStorage::~StarsStorage()
{}

const std::list<Star>& StarsStorage::get_stars()
{
  m_stars_mutex.lock();
  m_stars_tmp = m_stars;
  m_stars_mutex.unlock();
  return m_stars_tmp;
}

void StarsStorage::remove_oldest()
{
  m_stars_mutex.lock();
  std::list<Star> tmp = m_stars;
  m_stars_mutex.unlock();
  
  double seconds = m_star_time_of_live;
  tmp.remove_if([seconds](Star& s){
    return s.time_of_live() > seconds;
  });
  
  m_stars_mutex.lock();
  m_stars = tmp;
  m_stars_mutex.unlock();
  
  if (tmp.size() < m_star_count)
  {
    on_count_changed();
  }
}

void StarsStorage::on_count_changed()
{
  m_stars_mutex.lock();
  std::list<Star> tmp = m_stars;
  m_stars_mutex.unlock();
  
  int a = tmp.size() - m_star_count;
  if (a > 0)
  {
    tmp.sort([](Star& first, Star& second){
      return first.time_of_live() > second.time_of_live();
    });
    std::list<Star>::iterator it1, it2;
    it1 = it2 = tmp.begin();
    advance(it2, a);
    tmp.erase(it1, it2);
  }
  else if (a < 0)
  {
    for (int i = a; i < 0; i++)
    {
      tmp.push_back(new_star());
    }
  }
  
  if (a != 0)
  {
    m_stars_mutex.lock();
    m_stars = tmp;
    m_stars_mutex.unlock();
  }
}

const Star& StarsStorage::new_star()
{
  Color c;
  c.red   = (64 + rand() % 192) / 256.0f;
  c.green = (64 + rand() % 192) / 256.0f;
  c.blue  = (64 + rand() % 192) / 256.0f;
  c.alpha = 1.0f;
  
  Position p;
  
  float x = rand() % (int)(DEFAULT_SIZE_MAX) - DEFAULT_SIZE_MAX / 2;
  float y = rand() % (int)(DEFAULT_SIZE_MAX) - DEFAULT_SIZE_MAX / 2;
  float z = rand() % (int)(DEFAULT_SIZE_MAX) - DEFAULT_SIZE_MAX / 2;
  float a = atan(sqrt(x*x + y*y) / z) + ((rand() % 2) ? PI : 0);
  float b = atan(y/x);
  float r = (rand() % (int)(DEFAULT_SIZE_MAX - DEFAULT_SIZE_MIN)) + DEFAULT_SIZE_MIN;
  p.x = r * sin(a) * cos(b);
  p.y = r * sin(a) * sin(b);
  p.z = r * cos(a);
  
  Star::Size s;
  int v = rand() % 100;
  if (v < m_big_percent)
    s = Star::Size::big;
  else if(v < m_big_percent + m_middle_percent)
    s = Star::Size::middle;
  else
    s = Star::Size::small;
  
  clock_t t = clock() + rand() % (int)(m_star_time_of_live * CLOCKS_PER_SEC);
  float a_v = ANGLE_V_MIN + (rand() % (int)((ANGLE_V_MAX - ANGLE_V_MIN) * 100)) / 100.0;
  
  Star* st = new Star(c, p, s, m_star_time_of_live, t, a_v);
  
  return *st;
}
