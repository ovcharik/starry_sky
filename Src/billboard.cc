#include "billboard.h"

#include <cmath>
#include <iostream>

void Billboard::get_billboard(
  float x,
  float y,
  float z,
  float* cam,
  float size,
  float result[4][3])
{
  size = size / 2;
  
  result[0][0] = x + (-cam[0] - cam[1]) * size;
  result[0][1] = y + (-cam[4] - cam[5]) * size;
  result[0][2] = z + (-cam[8] - cam[9]) * size;
  
  result[1][0] = x + (cam[0] - cam[1]) * size;
  result[1][1] = y + (cam[4] - cam[5]) * size;
  result[1][2] = z + (cam[8] - cam[9]) * size;
  
  result[2][0] = x + (-cam[0] + cam[1]) * size;
  result[2][1] = y + (-cam[4] + cam[5]) * size;
  result[2][2] = z + (-cam[8] + cam[9]) * size;
  
  result[3][0] = x + (cam[0] + cam[1]) * size;
  result[3][1] = y + (cam[4] + cam[5]) * size;
  result[3][2] = z + (cam[8] + cam[9]) * size;
}

void Billboard::rotate(
  float x,
  float y,
  float z,
  float a,
  float b[4][3],
  float result[4][3])
{
  float rot[3][3];
  
  double s = std::sin(a);
  double c = std::cos(a);
  double ac = 1.0 - c;
  
  double l = std::sqrt(x*x + y*y + z*z);
  x /= l; y /= l; z /= l;
  
  rot[0][0] = c + ac*x*x  ; rot[0][1] = ac*x*y - s*z; rot[0][2] = ac*x*z + s*y;
  rot[1][0] = ac*y*x + s*z; rot[1][1] = c + ac*y*y  ; rot[1][2] = ac*y*z - s*x;
  rot[2][0] = ac*z*x - s*y; rot[2][1] = ac*z*y + s*x; rot[2][2] = c + ac*z*z  ;
  
  for (int i = 0; i < 4; i++)
  {
    result[i][0] = b[i][0]*rot[0][0] + b[i][1]*rot[0][1] + b[i][2]*rot[0][2];
    result[i][1] = b[i][0]*rot[1][0] + b[i][1]*rot[1][1] + b[i][2]*rot[1][2];
    result[i][2] = b[i][0]*rot[2][0] + b[i][1]*rot[2][1] + b[i][2]*rot[2][2];
  }
}
