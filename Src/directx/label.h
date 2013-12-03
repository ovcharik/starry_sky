#pragma once

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>

#include "i_window.h"

class Label : public IWindow
{
  HWND m_h_parent_wnd;
  HINSTANCE m_h_inst;
  
public:
  Label(HINSTANCE h_inst, HWND h_wnd, const char* value);
  virtual ~Label();
  
  void set_value(const char* value);
};
