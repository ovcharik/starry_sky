#include "app.h"

#include "../general/defines.h"
#include "../general/event_repository.h"

#include <iostream>

App::App(HINSTANCE h_inst, HINSTANCE h_prev_inst, LPSTR lp_cmd_line, int cmd_show)
  : m_h_inst(h_inst),
    m_cmd_show(cmd_show)
{
  ZeroMemory(&m_wndclass, sizeof(WNDCLASSEX));
  
  m_wndclass.cbSize        = sizeof(WNDCLASSEX);
  m_wndclass.style         = CS_HREDRAW | CS_VREDRAW;
  m_wndclass.lpfnWndProc   = (WNDPROC)IWindow::def_wnd_proc;
  m_wndclass.hInstance     = m_h_inst;
  //m_wndclass.hIcon
  m_wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
  m_wndclass.hbrBackground = (HBRUSH)COLOR_WINDOW;
  m_wndclass.lpszClassName = "WindowClass";
  //m_wndclass.hIconSm
  
  RegisterClassEx(&m_wndclass);
  
  m_h_wnd = CreateWindowEx(
    WS_EX_APPWINDOW,
    "WindowClass",
    APP_NAME,
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT,
    0, 0,
    NULL,
    NULL,
    m_h_inst,
    NULL);
  
  SetWindowLong(m_h_wnd, GWLP_USERDATA, (long)this);
  
  set_size(820, 510);
  
  // Creating childrens
  m_left_panel = new LeftPanel(m_h_inst, m_h_wnd);
  m_sky_scene  = new SkyScene(m_h_inst, m_h_wnd);
}

App::~App()
{
  delete m_left_panel;
  delete m_sky_scene;
}

int App::run()
{
  ShowWindow(m_h_wnd, m_cmd_show);
  
  MSG msg;
  while(true)
  {
    while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    if(msg.message == WM_QUIT)
      break;
  }
  return msg.wParam;
}

LRESULT App::wnd_proc(HWND h_wnd, UINT msg, WPARAM w_param, LPARAM l_param)
{
  switch(msg)
  {
  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  case WM_SIZE:
    m_left_panel->set_adjust(0, 0, 200, get_height());
    m_sky_scene->set_adjust(210, 5, get_width() - 215, get_height() - 10);
    break;
  case WM_KEYDOWN:
    switch (w_param)
    {
    case VK_UP:
      EventRepository::get_event<int>(EventTypes::WN_KEY_UP)->trigger(0);
      break;
    case VK_DOWN:
      EventRepository::get_event<int>(EventTypes::WN_KEY_DOWN)->trigger(0);
      break;
    case VK_LEFT:
      EventRepository::get_event<int>(EventTypes::WN_KEY_LEFT)->trigger(0);
      break;
    case VK_RIGHT:
      EventRepository::get_event<int>(EventTypes::WN_KEY_RIGHT)->trigger(0);
      break;
    case VK_PRIOR:
      EventRepository::get_event<int>(EventTypes::WN_KEY_PAGE_UP)->trigger(0);
      break;
    case VK_NEXT:
      EventRepository::get_event<int>(EventTypes::WN_KEY_PAGE_DOWN)->trigger(0);
      break;
    }
    return 0;
  }
  return DefWindowProc(h_wnd, msg, w_param, l_param);
}
