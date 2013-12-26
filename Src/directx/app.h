#pragma once

#include <windows.h>
#include <windowsx.h>

#include "i_window.h"

#include "left_panel.h"
#include "sky_scene.h"

class App : public IWindow
{
  HINSTANCE m_h_inst;
  WNDCLASSEX m_wndclass;
  int m_cmd_show;
  
  virtual LRESULT wnd_proc(HWND h_wnd, UINT msg, WPARAM w_param, LPARAM l_param);
  
public:
  App(HINSTANCE h_inst, HINSTANCE h_prev_inst, LPSTR lp_cmd_line, int cmd_show);
  virtual ~App();
  
  int run();
  
protected:
  LeftPanel* m_left_panel;
  SkyScene*  m_sky_scene;
  
  bool m_paused;
};
