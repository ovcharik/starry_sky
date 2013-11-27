#pragma once

#include <list>
#include <mutex>

#include "star.h"

class StarsStorage
{
public:
  StarsStorage();
  virtual ~StarsStorage();
  
protected:
  std::list<Star> m_stars;
  std::list<Star> m_stars_tmp;
  std::mutex m_stars_mutex;

public:
  const std::list<Star>& get_stars();

protected:
  void remove_oldest();
  const Star& new_star();
  
  void on_count_changed();
  
protected:
  float m_star_time_of_live;
  int m_star_count;
  int m_big_percent;
  int m_middle_percent;
  int m_small_percent;
};
