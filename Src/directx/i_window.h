#pragma once

#include <windows.h>
#include <windowsx.h>

class IWindow
{
protected:
  static LRESULT CALLBACK def_wnd_proc(
    HWND h_wnd,
    UINT msg,
    WPARAM w_param,
    LPARAM l_param);
    
  virtual LRESULT wnd_proc(
    HWND h_wnd,
    UINT msg,
    WPARAM w_param,
    LPARAM l_param);
  
  HWND m_h_wnd;

public:
  int get_width();
  void set_width(int width);
  
  int get_height();
  void set_height(int height);
  
  void set_size(int width, int height);
  void set_position(int x, int y);
  
  void set_adjust(int x = -1, int y = -1, int width = 0, int height = 0);
};
