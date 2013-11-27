#include "cloud_particle.h"

CloudParticle::CloudParticle(Position position, int texture, float size)
  : m_position(position),
    m_texture(texture),
    m_size(size)
{}

void CloudParticle::get_billboard(float* cam, float result[4][3])
{
  Billboard::get_billboard(
    m_position.x,
    m_position.y,
    m_position.z,
    cam, m_size,
    result);
}

void CloudParticle::get_texture(int width, int height, float points[4][2])
{
  int s = width * height;
  int p = m_texture % s;
  int x = p % width;
  int y = p / width;
  
  points[0][0] = 1.0 / width * (x + 1); points[0][1] = 1.0 / height * (y + 1);
  points[1][0] = 1.0 / width * (x + 0); points[1][1] = 1.0 / height * (y + 1);
  points[2][0] = 1.0 / width * (x + 1); points[2][1] = 1.0 / height * (y + 0);
  points[3][0] = 1.0 / width * (x + 0); points[3][1] = 1.0 / height * (y + 0);
}

const Position& CloudParticle::get_position()
{
  return m_position;
}
