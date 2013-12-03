#include "left_panel.h"

#include "../general/defines.h"
#include "../general/event_repository.h"

#include <iostream>
#include <cstdlib>
#include <cmath>

LeftPanel::LeftPanel(HINSTANCE h_inst, HWND h_wnd)
  : m_h_inst(h_inst),
    m_h_parent_wnd(h_wnd)
{
  
  WNDCLASSEX left_panel_wndclassex = {
    sizeof(WNDCLASSEX),
    CS_GLOBALCLASS,
    (WNDPROC)IWindow::def_wnd_proc,
    0, 0,
    m_h_inst,
    NULL,
    LoadCursor(NULL, IDC_ARROW),
    (HBRUSH)COLOR_WINDOW,
    NULL,
    "LeftPanelClass",
    NULL
  };
  
  RegisterClassEx(&left_panel_wndclassex);
  
  m_h_wnd = CreateWindowEx(
    0,
    "LeftPanelClass",
    "Left Panel",
    WS_CHILD | WS_VISIBLE,
    0, 0, // pos
    0, 0, // size
    m_h_parent_wnd,
    NULL,
    m_h_inst,
    NULL);
  
  SetWindowLong(m_h_wnd, GWLP_USERDATA, (long)this);
  
  // Labels
  m_labels = new Label*[6];
  m_labels[0] = new Label(m_h_inst, m_h_wnd, "Count of stars");
  m_labels[1] = new Label(m_h_inst, m_h_wnd, "Time of live");
  m_labels[2] = new Label(m_h_inst, m_h_wnd, "Big (%)");
  m_labels[3] = new Label(m_h_inst, m_h_wnd, "Middle (%)");
  m_labels[4] = new Label(m_h_inst, m_h_wnd, "Small (%)");
  m_labels[5] = new Label(m_h_inst, m_h_wnd, "FPS:");
  
  m_fps_label = new Label(m_h_inst, m_h_wnd, "0");
  
  // Trackbars
  m_count          = new Trackbar(m_h_inst, m_h_wnd, Trackbar::Types::COUNT,  0, 10000, DEFAULT_STAR_COUNT);
  m_time           = new Trackbar(m_h_inst, m_h_wnd, Trackbar::Types::TIME,   0, 60,    DEFAULT_STAR_TIME_OF_LIFE);
  m_percent_big    = new Trackbar(m_h_inst, m_h_wnd, Trackbar::Types::BIG,    0, 100,   DEFAULT_STAR_BIG);
  m_percent_middle = new Trackbar(m_h_inst, m_h_wnd, Trackbar::Types::MIDDLE, 0, 100,   DEFAULT_STAR_MIDDLE);
  m_percent_small  = new Trackbar(m_h_inst, m_h_wnd, Trackbar::Types::SMALL,  0, 100,   DEFAULT_STAR_SMALL);
  
  // Positions
  m_labels[0]->set_adjust(5, 5, 190, 20);
  m_count->set_adjust(5, 25, 190, 30);
  
  
  m_labels[1]->set_adjust(5, 65, 190, 20);
  m_time->set_adjust(5, 85, 190, 30);
  
  
  m_labels[2]->set_adjust(5, 125, 190, 20);
  m_percent_big->set_adjust(5, 145, 190, 30);
  
  m_labels[3]->set_adjust(5, 175, 190, 20);
  m_percent_middle->set_adjust(5, 195, 190, 30);
  
  m_labels[4]->set_adjust(5, 225, 190, 20);
  m_percent_small->set_adjust(5, 245, 190, 30);
  
  
  m_labels[5]->set_adjust(5, 285, 40, 20);
  m_fps_label->set_adjust(45, 285, 40, 20);
  
  // Conecting events
  EventRepository::get_event<double>(EventTypes::SS_FPS_CHANGED)
    ->add_handler([this](double value) {
      char str[20];
      sprintf(str, "%.0lf", value);
      this->m_fps_label->set_value(str);
    });
}

LeftPanel::~LeftPanel()
{
  delete m_count;
  delete m_time;
  delete m_percent_big;
  delete m_percent_middle;
  delete m_percent_small;
  
  for (int i = 0; i < 6; i++)
    delete m_labels[i];
  delete[] m_labels;
  
  delete m_fps_label;
}

LRESULT LeftPanel::wnd_proc(HWND h_wnd, UINT msg, WPARAM w_param, LPARAM l_param)
{
  switch (msg)
  {
  case WM_HSCROLL:
  case WM_VSCROLL:
    if (l_param != 0)
    {
      switch (LOWORD(w_param))
      {
      case TB_PAGEDOWN:
      case TB_PAGEUP:
      case TB_THUMBTRACK:
        {
          HWND trackbar_wnd = (HWND)l_param;
          int value = SendMessage(trackbar_wnd, TBM_GETPOS, 0, 0);
          Trackbar::Types type = (Trackbar::Types)GetDlgCtrlID(trackbar_wnd);
          switch (type)
          {
          case Trackbar::Types::COUNT:
            EventRepository::get_event<int>(EventTypes::LP_STAR_COUNT)->trigger(value);
            break;
          
          case Trackbar::Types::TIME:
            EventRepository::get_event<int>(EventTypes::LP_STAR_TIME_OF_LIVE)->trigger(value);
            break;
          
          case Trackbar::Types::BIG:
          case Trackbar::Types::MIDDLE:
          case Trackbar::Types::SMALL:
            {
              int b = m_percent_big->get_value();
              int m = m_percent_middle->get_value();
              int s = m_percent_small->get_value();
              int sum = b + m + s;
              int d = sum - 100;
              if (d != 0)
              {
                int sign = (d < 0) ? -1 : 1;
                d *= sign;
                int h1 = ((d / 2) + ((d % 2) ? rand() % 2 : 0));
                int h2 = (d - h1);
                h1 *= sign; h2 *= sign;
                
                if (type == Trackbar::Types::BIG)
                {
                  m -= h1; s -= h2;
                  if (m < 0)   { s += m;         m = 0;   }
                  if (m > 100) { s -= (m - 100); m = 100; }
                  if (s < 0)   { m += s;         s = 0;   }
                  if (s > 100) { m -= (s - 100); s = 100; }
                }
                if (type == Trackbar::Types::MIDDLE)
                {
                  b -= h1; s -= h2;
                  if (b < 0)   { s += b;         b = 0;   }
                  if (b > 100) { s -= (b - 100); b = 100; }
                  if (s < 0)   { b += s;         s = 0;   }
                  if (s > 100) { b -= (s - 100); s = 100; }
                }
                if (type == Trackbar::Types::SMALL)
                {
                  m -= h1; b -= h2;
                  if (m < 0)   { b += m;         m = 0;   }
                  if (m > 100) { b -= (m - 100); m = 100; }
                  if (b < 0)   { m += b;         b = 0;   }
                  if (b > 100) { m -= (b - 100); b = 100; }
                }
                
                m_percent_big->set_value(b);
                m_percent_middle->set_value(m);
                m_percent_small->set_value(s);
                
                EventRepository::get_event<int>(EventTypes::LP_PERCENT_BIG)->trigger(b);
                EventRepository::get_event<int>(EventTypes::LP_PERCENT_MIDDLE)->trigger(m);
                EventRepository::get_event<int>(EventTypes::LP_PERCENT_SMALL)->trigger(s);
              }
            }
            break;
          }
          break;
        }
      }
    }
    break;
  }
  return DefWindowProc(h_wnd, msg, w_param, l_param);
}
