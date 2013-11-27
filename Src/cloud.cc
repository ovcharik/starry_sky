#include "cloud.h"

#include <cstdlib>
#include <cmath>

Cloud::Cloud(Position position, float width, float height, float deep, int count)
  : m_position(position),
    m_width(width),
    m_height(height),
    m_deep(deep),
    m_particles_count(count)
{
  float h_w = width / 2;
  float h_h = height / 2;
  float h_d = deep / 2;
  
  float s = width * height / (float)count;
  float size[2];
  size[0] = s * 0.5; size[1] = s;
  
  for (int i = 0; i < count; i++)
  {
    Position p;
  
    float x = rand() % (int)(position.x + h_w) - h_w;
    float y = rand() % (int)(position.y + h_h) - h_h;
    float z = rand() % (int)(position.z + h_d) - h_d;
    float a = atan(sqrt(x*x + y*y) / z) + ((rand() % 2) ? PI : 0);
    float b = atan(y/x);
    float r = ((rand() % 2) ? 1 : -1) * (rand() % (int)(h_d)) + position.z;
    p.x = r * sin(a) * cos(b);
    p.y = r * sin(a) * sin(b);
    p.z = r * cos(a);
    
    s = size[0] + (rand() % (int)((size[1] - size[0]) * 100.0)) / 100.0;
    
    m_particles.push_back(CloudParticle(p, rand(), s));
  }
}

std::list<CloudParticle>& Cloud::get_particles()
{
  return m_particles;
}
