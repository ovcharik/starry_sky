#include "frustum.h"

#include <cmath>

void Frustum::get_frustum(float proj[16], float modl[16], float frustum[6][4])
{
  float clip[16];
  
  clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8] + modl[ 3] * proj[12];
  clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9] + modl[ 3] * proj[13];
  clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10] + modl[ 3] * proj[14];
  clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11] + modl[ 3] * proj[15];
  
  clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4] + modl[ 6] * proj[ 8] + modl[ 7] * proj[12];
  clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9] + modl[ 7] * proj[13];
  clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10] + modl[ 7] * proj[14];
  clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11] + modl[ 7] * proj[15];
  
  clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4] + modl[10] * proj[ 8] + modl[11] * proj[12];
  clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9] + modl[11] * proj[13];
  clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10] + modl[11] * proj[14];
  clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11] + modl[11] * proj[15];
  
  clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4] + modl[14] * proj[ 8] + modl[15] * proj[12];
  clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] + modl[15] * proj[13];
  clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] + modl[15] * proj[14];
  clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] + modl[15] * proj[15];
  
  /* ПРАВАЯ плоскость */
  frustum[0][0] = clip[ 3] - clip[ 0];
  frustum[0][1] = clip[ 7] - clip[ 4];
  frustum[0][2] = clip[11] - clip[ 8];
  frustum[0][3] = clip[15] - clip[12];
  /* ЛЕВАЯ плоскость */
  frustum[1][0] = clip[ 3] + clip[ 0];
  frustum[1][1] = clip[ 7] + clip[ 4];
  frustum[1][2] = clip[11] + clip[ 8];
  frustum[1][3] = clip[15] + clip[12];
  
  /* НИЖНЯЯ плоскость */
  frustum[2][0] = clip[ 3] + clip[ 1];
  frustum[2][1] = clip[ 7] + clip[ 5];
  frustum[2][2] = clip[11] + clip[ 9];
  frustum[2][3] = clip[15] + clip[13];
  
  /* ВЕРХНЯЯ плоскость */
  frustum[3][0] = clip[ 3] - clip[ 1];
  frustum[3][1] = clip[ 7] - clip[ 5];
  frustum[3][2] = clip[11] - clip[ 9];
  frustum[3][3] = clip[15] - clip[13];
  
  /* ЗАДНЯЯ плоскость */
  frustum[4][0] = clip[ 3] - clip[ 2];
  frustum[4][1] = clip[ 7] - clip[ 6];
  frustum[4][2] = clip[11] - clip[10];
  frustum[4][3] = clip[15] - clip[14];
  
  /* ПЕРЕДНЯЯ плоскость */
  frustum[5][0] = clip[ 3] + clip[ 2];
  frustum[5][1] = clip[ 7] + clip[ 6];
  frustum[5][2] = clip[11] + clip[10];
  frustum[5][3] = clip[15] + clip[14];
  
  for (int i = 0; i < 6; i++)
  {
    float t = sqrt(frustum[i][0] * frustum[i][0] + frustum[i][1] * frustum[i][1] + frustum[i][2] * frustum[i][2]);
    for (int j = 0; j < 4; j++)
      frustum[i][j] /= t;
  }
}

bool Frustum::point_in_frustum(float x, float y, float z, float frustum[6][4])
{
  for(int p = 0; p < 6; p++)
    if( frustum[p][0] * x + frustum[p][1] * y + frustum[p][2] * z + frustum[p][3] <= 0 )
      return false;
  return true;
}

bool Frustum::rect_in_frustum(float rect[4][3], float frustum[6][4])
{
  for (int i = 0; i < 4; i++)
    if (point_in_frustum(rect[i][0], rect[i][1], rect[i][2], frustum))
      return true;
  return false;
}
