#pragma once

#include "billboard.h"
#include "defines.h"

class CloudParticle : public Billboard
{
  Position m_position;
  int m_texture;
  float m_size;
  
public:
  
  CloudParticle(Position position, int texture, float size);
  
  void get_billboard(float* cam, float result[4][3]);
  void get_texture(int width, int height, float points[4][2]);
  const Position& get_position();
};
