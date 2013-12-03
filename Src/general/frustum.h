#pragma once

class Frustum
{
public:
  static void get_frustum(float proj[16], float modl[16], float frustum[6][4]);
  static bool point_in_frustum(float x, float y, float z, float frustum[6][4]);
  static bool rect_in_frustum(float rect[4][3], float frustum[6][4]);
};
