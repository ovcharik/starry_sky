#pragma once

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>

#include "i_window.h"

class Trackbar : public IWindow
{
public:
  enum Types
  {
    COUNT  = 0x101,
    TIME   = 0x102,
    BIG    = 0x103,
    MIDDLE = 0x104,
    SMALL  = 0x105
  };
  
private:
  Trackbar::Types m_type;
  HWND m_h_parent_wnd;
  HINSTANCE m_h_inst;
  
public:
  Trackbar(
    HINSTANCE h_inst,
    HWND h_parent_wnd,
    Trackbar::Types type,
    int min,
    int max,
    int value);
  
  virtual ~Trackbar();
  
  int get_value();
  void set_value(int value);
};
