#pragma once

#include <windows.h>
#include <windowsx.h>

#include "i_window.h"

#include "trackbar.h"
#include "label.h"

class LeftPanel : public IWindow
{
public:
  LeftPanel(HINSTANCE h_inst, HWND h_wnd);
  virtual ~LeftPanel();
  
  virtual LRESULT wnd_proc(HWND h_wnd, UINT msg, WPARAM w_param, LPARAM l_param);

protected:
  HINSTANCE m_h_inst;
  HWND m_h_parent_wnd;

protected:
  Trackbar* m_count;
  Trackbar* m_time;
  Trackbar* m_percent_big;
  Trackbar* m_percent_middle;
  Trackbar* m_percent_small;
  
  Label**   m_labels;
  Label*    m_fps_label;
};
