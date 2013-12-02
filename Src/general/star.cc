#include "star.h"

Star::Star(
  const Color& color,
  const Position& position,
  const Star::Size& size,
  const float seconds_of_live,
  const clock_t& clocks,
  const float angle_v)
  : Billboard(),
    m_color(color),
    m_position(position),
    m_size(size),
    m_clocks_of_born(clocks),
    m_real_clocks_of_born(clock()),
    m_seconds_of_live(seconds_of_live),
    m_angle_v(angle_v)
{}

Star::~Star()
{
}


const Color& Star::get_color()
{
  this->update_alpha();
  return this->m_color;
}

const Position& Star::get_position()
{
  return this->m_position;
}

const Star::Size& Star::get_size()
{
  return this->m_size;
}


void Star::set_color(const Color& color)
{
  this->m_color = color;
}

void Star::set_position(const Position& position)
{
  this->m_position = position;
}

void Star::set_size(const Star::Size& size)
{
  this->m_size = size;
}

double Star::time_of_live()
{
  return (clock() - m_clocks_of_born) / (double)CLOCKS_PER_SEC;
}

float Star::get_angle()
{
  return time_of_live() * m_angle_v;
}

float Star::get_size(float b, float m, float s)
{
  return (m_size == Size::big) ? b : (m_size == Size::middle) ? m : s;
}

#include <iostream>
void Star::update_alpha()
{
  double t = (clock() - m_real_clocks_of_born) / (double)CLOCKS_PER_SEC;
  double l = 2.0;
  if (t > 2.0)
  {
    t = time_of_live();
    double a = m_seconds_of_live - 2.0;
    if (t > a)
      l = 2.0 - t + a;
  }
  else
    l = t;
  m_color.alpha = l / 2.0;
}

void Star::get_billboard(float* cam, float size, float result[4][3])
{
  Billboard::get_billboard(
    m_position.x,
    m_position.y,
    m_position.z,
    cam, size,
    result);
}

void Star::get_roted_billboard(float billboard[4][3], float result[4][3])
{
  Billboard::rotate(
    m_position.x,
    m_position.y,
    m_position.z,
    get_angle(),
    billboard,
    result);
}
