#pragma once

#include <vector>
#include <ctime>

#include "defines.h"
#include "billboard.h"

class Star : public Billboard
{
public:
  enum Size
  {
    SMALL,
    MIDDLE,
    BIG
  };
  
protected:
  Color m_color;
  Size m_size;
  Position m_position;
  
  float m_angle_v;
  float m_seconds_of_live;
  
  clock_t m_clocks_of_born;
  clock_t m_real_clocks_of_born;
  
public:
  Star(
    const Color& color,
    const Position& position,
    const Star::Size& size,
    const float seconds_of_live,
    const clock_t& clocks = clock(),
    const float angle_v = 0.5);
  virtual ~Star();
  
  const Color& get_color();
  void set_color(const Color& color);
  
  const Position& get_position();
  void set_position(const Position& position);
  
  const Size& get_size();
  void set_size(const Star::Size& size);
  
  double time_of_live();
  float  get_angle();
  
  float get_size(float b, float m, float s);
  void get_billboard(float* cam, float size, float result[4][3]);
  void get_roted_billboard(float billboard[4][3], float result[4][3]);
  void update_alpha();
};
