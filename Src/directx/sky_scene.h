#pragma once

#include "i_window.h"

#include "../general/defines.h"
#include "../general/stars_storage.h"
#include "../general/cloud.h"

#include <windows.h>
#include <windowsx.h>

#include <d3d9.h>
#include <d3dx9.h>

#include <ctime>

class SkyScene : public IWindow
{
public:
  SkyScene(HINSTANCE h_inst, HWND h_parent_wnd);
  virtual ~SkyScene();
  
protected:
  virtual LRESULT wnd_proc(HWND h_wnd, UINT msg, WPARAM w_param, LPARAM l_param);
  
  HINSTANCE m_h_inst;
  HWND m_h_parent_wnd;
  
  LPDIRECT3D9 m_d3d_interface;
  LPDIRECT3DDEVICE9 m_d3d_device;
  LPDIRECT3DVERTEXBUFFER9 m_v_buffer;
  LPDIRECT3DINDEXBUFFER9 m_i_buffer;
  
  bool m_inited;
  
  struct CUSTOMVERTEX {FLOAT X, Y, Z; DWORD COLOR;};
  #define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)
  
protected:
  void on_create();
  void on_resize();
  
public:
  void render();
  
protected:
  float m_view_rot_x;
  float m_view_rot_y;
  float m_view_rot_z;
  
  float m_view_pos_z;
  
  StarsStorage m_stars_storage;
  Cloud m_cloud;
  
protected:
  int m_frames;
  unsigned long m_clocks;
  unsigned long m_prev_clocks;
};
