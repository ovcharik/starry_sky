#pragma once

class Billboard
{
protected:
  
  void get_billboard(
    float x,
    float y,
    float z,
    float* cam,
    float size,
    float result[4][3]);
  
  void rotate(
    float x,
    float y,
    float z,
    float angle,
    float billboard[4][3],
    float result[4][3]);
};
