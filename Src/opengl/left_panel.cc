#include "left_panel.h"

#include "../general/defines.h"
#include "../general/event_repository.h"

#include <iostream>
#include <functional>
#include <cmath>
#include <cstdio>

LeftPanel::LeftPanel()
  : Gtk::VBox(),
    
    m_adj_count( DEFAULT_STAR_COUNT, 0, 10000, 100 ),
    m_scale_count(m_adj_count),
    
    m_adj_time( DEFAULT_STAR_TIME_OF_LIFE, 1, 60, 1 ),
    m_scale_time(m_adj_time),
    
    m_adj_percent_big( DEFAULT_STAR_BIG, 0, 100, 5 ),
    m_scale_percent_big(m_adj_percent_big),
    m_adj_percent_middle( DEFAULT_STAR_MIDDLE, 0, 100, 5 ),
    m_scale_percent_middle(m_adj_percent_middle),
    m_adj_percent_small( DEFAULT_STAR_SMALL, 0, 100, 5 ),
    m_scale_percent_small(m_adj_percent_small),
    
    m_fps("0", 0, 0)
{
  EventRepository& er = *EventRepository::get_instance();
  
  // defaults
  set_size_request(200);
  
  m_scale_count.set_digits(0);
  
  m_scale_percent_big.set_digits(0);
  m_scale_percent_middle.set_digits(0);
  m_scale_percent_small.set_digits(0);
  
  // connecting events
  auto count_event = er.get_event<float>(EventTypes::LP_STAR_COUNT);
  this->m_adj_count_connection = m_adj_count.signal_value_changed()
    .connect([this, count_event]()
  {
    count_event->trigger(this->m_adj_count.get_value());
  });
  
  
  auto time_event = er.get_event<float>(EventTypes::LP_STAR_TIME_OF_LIVE);
  this->m_adj_time_connection = m_adj_time.signal_value_changed()
    .connect([this, time_event]()
  {
    time_event->trigger(this->m_adj_time.get_value());
  });
  
  
  
  auto percent_big_event = er.get_event<float>(EventTypes::LP_PERCENT_BIG);
  this->m_adj_percent_big_connection = m_adj_percent_big.signal_value_changed()
    .connect([this, percent_big_event]()
  {
    percent_big_event->trigger(this->m_adj_percent_big.get_value());
    double b = this->m_adj_percent_big.get_value(),
      m = this->m_adj_percent_middle.get_value(),
      s = this->m_adj_percent_small.get_value(),
      all = b + m + s,
      d = all - 100;
    if (std::abs(d) > 0.5)
    {
      double a = d / 2;
      this->m_adj_percent_middle.set_value((int)(m - a));
      this->m_adj_percent_small.set_value((int)(s - a));
    }
  });
  auto percent_middle_event = er.get_event<float>(EventTypes::LP_PERCENT_MIDDLE);
  this->m_adj_percent_middle_connection = m_adj_percent_middle.signal_value_changed()
    .connect([this, percent_middle_event]()
  {
    percent_middle_event->trigger(this->m_adj_percent_middle.get_value());
    double b = this->m_adj_percent_big.get_value(),
      m = this->m_adj_percent_middle.get_value(),
      s = this->m_adj_percent_small.get_value(),
      all = b + m + s,
      d = all - 100;
    if (std::abs(d) > 0.5)
    {
      double a = d / 2;
      this->m_adj_percent_big.set_value((int)(b - a));
      this->m_adj_percent_small.set_value((int)(s - a));
    }
  });
  auto percent_small_event = er.get_event<float>(EventTypes::LP_PERCENT_SMALL);
  this->m_adj_percent_small_connection = m_adj_percent_small.signal_value_changed()
    .connect([this, percent_small_event]()
  {
    percent_small_event->trigger(this->m_adj_percent_small.get_value());
    double b = this->m_adj_percent_big.get_value(),
      m = this->m_adj_percent_middle.get_value(),
      s = this->m_adj_percent_small.get_value(),
      all = b + m + s,
      d = all - 100;
    if (std::abs(d) > 0.5)
    {
      double a = d / 2;
      this->m_adj_percent_big.set_value((int)(b - a));
      this->m_adj_percent_middle.set_value((int)(m - a));
    }
  });
  
  
  er.get_event<double>(EventTypes::SS_FPS_CHANGED)->add_handler(
    [this](double fps){
      //std::cout << fps << std::endl;
      char str[20];
      sprintf(str, "%.0lf", fps);
      this->m_fps.set_text(str);
    });
  
  // packing controlls
  pack_start(
    *Gtk::manage(new Gtk::Label("Count of stars", 0, 0)),
    Gtk::PACK_SHRINK);
  pack_start(m_scale_count, Gtk::PACK_SHRINK);
  
  pack_start(
    *Gtk::manage(new Gtk::HSeparator),
    Gtk::PACK_SHRINK, 5);
  
  pack_start(
    *Gtk::manage(new Gtk::Label("Time of live", 0, 0)),
    Gtk::PACK_SHRINK);
  pack_start(m_scale_time, Gtk::PACK_SHRINK);
  
  pack_start(
    *Gtk::manage(new Gtk::HSeparator),
    Gtk::PACK_SHRINK, 5);
  
  pack_start(
    *Gtk::manage(new Gtk::Label("Big (%)", 0, 0)),
    Gtk::PACK_SHRINK);
  pack_start(m_scale_percent_big, Gtk::PACK_SHRINK);
  pack_start(
    *Gtk::manage(new Gtk::Label("Middle (%)", 0, 0)),
    Gtk::PACK_SHRINK);
  pack_start(m_scale_percent_middle, Gtk::PACK_SHRINK);
  pack_start(
    *Gtk::manage(new Gtk::Label("Small (%)", 0, 0)),
    Gtk::PACK_SHRINK);
  pack_start(m_scale_percent_small, Gtk::PACK_SHRINK);
  
  
  Gtk::HBox* h_box = Gtk::manage(new Gtk::HBox());
  h_box->set_spacing(10);
  h_box->pack_start(
    *Gtk::manage(new Gtk::Label("FPS:", 0, 0)),
    Gtk::PACK_SHRINK);
  h_box->pack_start(m_fps, Gtk::PACK_SHRINK);
  
  pack_start(
    *Gtk::manage(new Gtk::HSeparator),
    Gtk::PACK_SHRINK, 5);
  
  pack_end(*h_box);
}

LeftPanel::~LeftPanel()
{
  this->m_adj_count_connection.disconnect();
  
  this->m_adj_time_connection.disconnect();
  
  this->m_adj_percent_big_connection.disconnect();
  this->m_adj_percent_middle_connection.disconnect();
  this->m_adj_percent_small_connection.disconnect();
}
