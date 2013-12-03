#include "i_window.h"

LRESULT CALLBACK IWindow::def_wnd_proc(
    HWND h_wnd,
    UINT msg,
    WPARAM w_param,
    LPARAM l_param)
{
  IWindow* wnd = (IWindow*)GetWindowLong(h_wnd, GWLP_USERDATA);
  if (wnd == NULL)
  {
    return DefWindowProc(h_wnd, msg, w_param, l_param);
  }
  
  return wnd->wnd_proc(h_wnd, msg, w_param, l_param);
}

LRESULT IWindow::wnd_proc(
    HWND h_wnd,
    UINT msg,
    WPARAM w_param,
    LPARAM l_param)
{
  return DefWindowProc(h_wnd, msg, w_param, l_param);
}

int IWindow::get_width()
{
  RECT rect;
  GetClientRect(m_h_wnd, &rect);
  return rect.right - rect.left;
}

void IWindow::set_width(int width)
{
  set_size(width, 0);
}

int IWindow::get_height()
{
  RECT rect;
  GetClientRect(m_h_wnd, &rect);
  return rect.bottom - rect.top;
}

void IWindow::set_height(int height)
{
  set_size(0, height);
}

void IWindow::set_size(int width, int height)
{
  set_adjust(-1, -1, width, height);
}

void IWindow::set_position(int x, int y)
{
  set_adjust(x, y);
}

void IWindow::set_adjust(int x, int y, int width, int height)
{
  RECT w_rect, c_rect;
  
  GetClientRect(m_h_wnd, &c_rect);
  GetWindowRect(m_h_wnd, &w_rect);
  
  int c_w = c_rect.right - c_rect.left;
  int c_h = c_rect.bottom - c_rect.top;
  
  int w_w = w_rect.right - w_rect.left;
  int w_h = w_rect.bottom - w_rect.top;
  
  int d_w = w_w - c_w;
  int d_h = w_h - c_h;
  
  if (width == 0)
    width = c_w;
  if (height == 0)
    height = c_h;
  if (x < 0)
    x = w_rect.left;
  if (y < 0)
    y = w_rect.top;
  
  MoveWindow(
    m_h_wnd,
    x,
    y,
    width + d_w,
    height + d_h,
    TRUE);
}
