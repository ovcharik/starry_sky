#include "trackbar.h"

Trackbar::Trackbar(HINSTANCE h_inst, HWND h_parent_wnd, Trackbar::Types type, int min, int max, int value)
  : m_h_inst(h_inst),
    m_h_parent_wnd(h_parent_wnd),
    m_type(type)
{
  InitCommonControls();
  
  m_h_wnd = CreateWindowEx(
    0,
    TRACKBAR_CLASS,
    "Trackbar Control",
    WS_CHILD | WS_VISIBLE | TBS_NOTICKS,
    0, 0,
    0, 0,
    m_h_parent_wnd,
    (HMENU)m_type,
    m_h_inst,
    NULL);
  
  SendMessage(
    m_h_wnd,
    TBM_SETRANGE,
    (WPARAM) FALSE,
    (LPARAM) MAKELONG(min, max));
  
  SendMessage(
    m_h_wnd,
    TBM_SETPAGESIZE,
    (WPARAM) FALSE,
    (LPARAM) ((max - min) / 10));
  
  set_value(value);
}

Trackbar::~Trackbar()
{
}

int Trackbar::get_value()
{
  return SendMessage(m_h_wnd, TBM_GETPOS, 0, 0);
}

void Trackbar::set_value(int value)
{
  SendMessage(
    m_h_wnd,
    TBM_SETPOS,
    (WPARAM) TRUE,
    (LPARAM) value);
}
