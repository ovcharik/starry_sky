#pragma once

#include "cloud_particle.h"
#include "defines.h"


#include <list>
#include <cstdlib>

class Cloud
{
  std::list<CloudParticle> m_particles;
  Position m_position;
  float m_width;
  float m_height;
  float m_deep;
  int m_particles_count;
  
public:
  Cloud(Position position, float width, float height, float deep, int count);
  
  std::list<CloudParticle>& get_particles();
};
