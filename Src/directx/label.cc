#include "label.h"

Label::Label(HINSTANCE h_inst, HWND h_wnd, const char* value)
  : m_h_inst(h_inst),
    m_h_parent_wnd(h_wnd)
{
  InitCommonControls();
  
  m_h_wnd = CreateWindowEx(
    0,
    "STATIC",
    "Label Control",
    WS_CHILD | WS_VISIBLE | SS_LEFT | SS_SIMPLE,
    0, 0,
    0, 0,
    m_h_parent_wnd,
    NULL,
    m_h_inst,
    NULL);
  
  set_value(value);
}

Label::~Label()
{
}

void Label::set_value(const char* value)
{
  UpdateWindow(m_h_wnd);
  LPRECT rect = NULL;
  GetWindowRect(m_h_wnd, rect);
  InvalidateRect(m_h_parent_wnd, rect, TRUE);
  SetWindowText(m_h_wnd, value);
}
