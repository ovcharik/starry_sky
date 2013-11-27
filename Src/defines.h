#pragma once

#ifdef GL
#define APP_NAME "StarrySky-OpenGL"
#elif DX
#define APP_NAME "StarrySky-DirectX"
#else
#define APP_NAME "StarrySky"
#endif

#define DEFAULT_ROT_X 0.0f
#define DEFAULT_ROT_Y 0.0f
#define DEFAULT_ROT_Z 0.0f

#define DEFAULT_POS_Z -750.0f

#define DELTA_ROT 0.5f
#define DELTA_POS 10.0f

#define DEFAULT_STAR_COUNT 2500
#define DEFAULT_STAR_TIME_OF_LIFE 60.0
#define DEFAULT_STAR_BIG 64.0
#define DEFAULT_STAR_MIDDLE 16.0
#define DEFAULT_STAR_SMALL 16.0

#define DEFAULT_SIZE_MAX 400.0f
#define DEFAULT_SIZE_MIN 300.0f

#define PI 3.14159265

#define ANGLE_V_MIN 0.2
#define ANGLE_V_MAX 1.0

#define MAX_FPS 60.0
#define MAX_SEC_ON_FRAME 0.0167

struct Color
{
  float red;
  float green;
  float blue;
  float alpha;
};

struct Position
{
  Position(){;}
  Position(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {;}
  Position(const Position& p) : x(p.x), y(p.y), z(p.z) {;}
  float x;
  float y;
  float z;
};
