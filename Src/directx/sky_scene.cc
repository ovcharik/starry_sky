#include "sky_scene.h"

#include "../general/event_repository.h"

#include <iostream>

SkyScene::SkyScene(HINSTANCE h_inst, HWND h_parent_wnd)
  : m_h_inst(h_inst),
    m_h_parent_wnd(h_parent_wnd),
    
    m_view_rot_x(DEFAULT_ROT_X),
    m_view_rot_y(DEFAULT_ROT_Y),
    m_view_rot_z(DEFAULT_ROT_Z),
    
    m_view_pos_z(DEFAULT_POS_Z),
    
    m_cloud(Position(0, 0, 350), 200, 100, 100, 100),
    
    m_frames(0),
    m_clocks(GetTickCount()),
    m_prev_clocks(GetTickCount())
{
  WNDCLASSEX sky_scene_wndclassex = {
    sizeof(WNDCLASSEX),
    CS_HREDRAW | CS_VREDRAW,
    (WNDPROC)IWindow::def_wnd_proc,
    0, 0,
    m_h_inst,
    NULL,
    LoadCursor(NULL, IDC_ARROW),
    CreateSolidBrush(RGB(0, 0, 0)),
    NULL,
    "SkySceneClass",
    NULL
  };
  
  RegisterClassEx(&sky_scene_wndclassex);
  
  m_h_wnd = CreateWindowEx(
    0,
    "SkySceneClass",
    "Sky Scene",
    WS_CHILD | WS_VISIBLE,
    0, 0, // pos
    0, 0, // size
    m_h_parent_wnd,
    NULL,
    m_h_inst,
    NULL);
  
  SetWindowLong(m_h_wnd, GWLP_USERDATA, (long)this);
  
  // Attach events
  EventRepository::get_event<int>(EventTypes::WN_KEY_UP)
    ->add_handler([this](int key){
      this->m_view_rot_x -= DELTA_ROT;
      this->invalidate();
    });
  EventRepository::get_event<int>(EventTypes::WN_KEY_DOWN)
    ->add_handler([this](int key){
      this->m_view_rot_x += DELTA_ROT;
      this->invalidate();
    });
  EventRepository::get_event<int>(EventTypes::WN_KEY_LEFT)
    ->add_handler([this](int key){
      this->m_view_rot_y -= DELTA_ROT;
      this->invalidate();
    });
  EventRepository::get_event<int>(EventTypes::WN_KEY_RIGHT)
    ->add_handler([this](int key){
      this->m_view_rot_y += DELTA_ROT;
      this->invalidate();
    });
  
  EventRepository::get_event<int>(EventTypes::WN_KEY_PAGE_UP)
    ->add_handler([this](int key){
      this->m_view_pos_z += DELTA_POS;
      this->invalidate();
    });
  EventRepository::get_event<int>(EventTypes::WN_KEY_PAGE_DOWN)
    ->add_handler([this](int key){
      this->m_view_pos_z -= DELTA_POS;
      this->invalidate();
    });
  
  EventRepository::get_event<float>(EventTypes::LP_STAR_COUNT)
    ->add_handler([this](float value){
      this->invalidate();
    });
}

SkyScene::~SkyScene()
{
}

LRESULT SkyScene::wnd_proc(HWND h_wnd, UINT msg, WPARAM w_param, LPARAM l_param)
{
  switch (msg)
  {
  case WM_CREATE:
    on_create();
    break;
  case WM_SIZE:
    on_resize();
    break;
  case WM_PAINT:
    on_redraw();
    break;
  }
  return DefWindowProc(h_wnd, msg, w_param, l_param);
}


void SkyScene::on_create()
{
  std::cout << "Sky scene created" << std::endl;
}

void SkyScene::on_resize()
{
  std::cout << "Sky scene resized" << std::endl;
}

void SkyScene::on_redraw()
{
  // Calculating fps
  m_frames++;
  unsigned long t = GetTickCount();
  double seconds = (t - m_clocks) / 1000.0;
  if (seconds > 0.5)
  {
    double fps = m_frames / seconds;
    m_frames = 0;
    m_clocks = t;
    
    EventRepository::get_event<int>(EventTypes::SS_TIMER)->trigger(0);
    EventRepository::get_event<double>(EventTypes::SS_FPS_CHANGED)->trigger(fps);
  }
  
  // Control fps
  double d = (t - m_prev_clocks) / 1000.0;
  m_prev_clocks = t;
  std::cout << d << std::endl;
  if (d < MAX_SEC_ON_FRAME)
  {
    d = (MAX_SEC_ON_FRAME - d) * 1000;
    std::cout << (unsigned long)d << std::endl;
    Sleep((unsigned long)d);
  }
}


void SkyScene::invalidate()
{
  UpdateWindow(m_h_wnd); 
}

























