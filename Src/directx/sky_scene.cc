#include "sky_scene.h"

#include "../general/event_repository.h"
#include "../general/frustum.h"

#include <iostream>

SkyScene::SkyScene(HINSTANCE h_inst, HWND h_parent_wnd)
  : m_h_inst(h_inst),
    m_h_parent_wnd(h_parent_wnd),
    
    m_view_rot_x(DEFAULT_ROT_X),
    m_view_rot_y(DEFAULT_ROT_Y),
    m_view_rot_z(DEFAULT_ROT_Z),
    
    m_view_pos_z(10.0f),
    
    m_cloud(Position(0, 0, 350), 200, 100, 100, 100),
    
    m_frames(0),
    m_clocks(GetTickCount()),
    m_prev_clocks(GetTickCount()),
    
    m_inited(false)
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
      this->render();
    });
  EventRepository::get_event<int>(EventTypes::WN_KEY_DOWN)
    ->add_handler([this](int key){
      this->m_view_rot_x += DELTA_ROT;
      this->render();
    });
  EventRepository::get_event<int>(EventTypes::WN_KEY_LEFT)
    ->add_handler([this](int key){
      this->m_view_rot_y -= DELTA_ROT;
      this->render();
    });
  EventRepository::get_event<int>(EventTypes::WN_KEY_RIGHT)
    ->add_handler([this](int key){
      this->m_view_rot_y += DELTA_ROT;
      this->render();
    });
  
  EventRepository::get_event<int>(EventTypes::WN_KEY_PAGE_UP)
    ->add_handler([this](int key){
      this->m_view_pos_z += DELTA_POS;
      this->render();
    });
  EventRepository::get_event<int>(EventTypes::WN_KEY_PAGE_DOWN)
    ->add_handler([this](int key){
      this->m_view_pos_z -= DELTA_POS;
      this->render();
    });
  
  EventRepository::get_event<float>(EventTypes::LP_STAR_COUNT)
    ->add_handler([this](float value){
      this->render();
    });
}

SkyScene::~SkyScene()
{
  if (m_inited)
  {
    m_v_buffer->Release();
    m_i_buffer->Release();
    m_d3d_device->Release();
    m_d3d_interface->Release();
  }
}

LRESULT SkyScene::wnd_proc(HWND h_wnd, UINT msg, WPARAM w_param, LPARAM l_param)
{
  switch (msg)
  {
  case WM_CREATE:
    break;
  case WM_SIZE:
    on_create();
    on_resize();
    break;
  }
  return DefWindowProc(h_wnd, msg, w_param, l_param);
}


void SkyScene::on_create()
{
  m_d3d_interface = Direct3DCreate9(D3D_SDK_VERSION);
  
  D3DPRESENT_PARAMETERS d3dpp;
  ZeroMemory(&d3dpp, sizeof(d3dpp));
  
  d3dpp.Windowed = TRUE;
  d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
  d3dpp.hDeviceWindow = m_h_wnd;
  d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
  d3dpp.BackBufferWidth = get_width();
  d3dpp.BackBufferHeight = get_height();
  d3dpp.EnableAutoDepthStencil = TRUE;
  d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
  
  m_d3d_interface->CreateDevice(
    D3DADAPTER_DEFAULT,
    D3DDEVTYPE_HAL,
    m_h_wnd,
    D3DCREATE_SOFTWARE_VERTEXPROCESSING,
    &d3dpp,
    &m_d3d_device);
  
  CUSTOMVERTEX vertices[] =
  {
    { -3.0f, 0.0f,  3.0f, D3DCOLOR_XRGB(0, 255, 0  ), },
    {  3.0f, 0.0f,  3.0f, D3DCOLOR_XRGB(0,   0, 255), },
    { -3.0f, 0.0f, -3.0f, D3DCOLOR_XRGB(255, 0, 0  ), },
    {  3.0f, 0.0f, -3.0f, D3DCOLOR_XRGB(0, 255, 255), },
    
    {  0.0f, 7.0f,  0.0f, D3DCOLOR_XRGB(0, 255, 0  ), },
  };
  
  m_d3d_device->CreateVertexBuffer(
    5 * sizeof(CUSTOMVERTEX),
    0,
    CUSTOMFVF,
    D3DPOOL_MANAGED,
    &m_v_buffer,
    NULL);
  
  VOID* pVoid;
  m_v_buffer->Lock(0, 0, (void**)&pVoid, 0);
  memcpy(pVoid, vertices, sizeof(vertices));
  m_v_buffer->Unlock();
  
  short indices[] =
  {
    0, 2, 1,    // base
    1, 2, 3,
    0, 1, 4,    // sides
    1, 3, 4,
    3, 2, 4,
    2, 0, 4,
  };

  m_d3d_device->CreateIndexBuffer(
    18 * sizeof(short),
    0,
    D3DFMT_INDEX16,
    D3DPOOL_MANAGED,
    &m_i_buffer,
    NULL);
  
  m_i_buffer->Lock(0, 0, (void**)&pVoid, 0);
  memcpy(pVoid, indices, sizeof(indices));
  m_i_buffer->Unlock();
  
  m_d3d_device->SetRenderState(D3DRS_LIGHTING, FALSE);
  m_d3d_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
  m_d3d_device->SetRenderState(D3DRS_ZENABLE, TRUE);
  
  m_inited = true;
}

void SkyScene::on_resize()
{
  std::cout << "Sky scene resized" << std::endl;
}

void SkyScene::render()
{
  // Render scene
  
  if (!m_inited)
    return;
  
  m_d3d_device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
  m_d3d_device->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
  
  m_d3d_device->BeginScene();
  
  m_d3d_device->SetFVF(CUSTOMFVF);
  
  D3DXMATRIX matRotateWorld, matRotateX, matRotateY, matRotateZ;
  D3DXMatrixRotationX(&matRotateX, m_view_rot_x);
  D3DXMatrixRotationY(&matRotateY, m_view_rot_y);
  D3DXMatrixRotationZ(&matRotateZ, m_view_rot_z);
  D3DXMatrixMultiply(&matRotateWorld, &matRotateX, &matRotateY);
  D3DXMatrixMultiply(&matRotateWorld, &matRotateWorld, &matRotateZ);
  m_d3d_device->SetTransform(D3DTS_WORLD, &matRotateWorld);
  
  D3DXMATRIX matView;
  D3DXVECTOR3 cam(0.0f, 0.0f, m_view_pos_z);
  D3DXVECTOR3 look(0.0f, 0.0f, 0.0f);
  D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
  D3DXMatrixLookAtLH(
    &matView,
    &cam,
    &look,
    &up);
  m_d3d_device->SetTransform(D3DTS_VIEW, &matView);
  
  D3DXMATRIX matProjection;
  D3DXMatrixPerspectiveFovLH(
    &matProjection,
    D3DXToRadian(45),
    (float)get_width() / (float)get_height(),
    1.0f,
    100.0f);
  m_d3d_device->SetTransform(D3DTS_PROJECTION, &matProjection);
  
  
  m_d3d_device->SetStreamSource(0, m_v_buffer, 0, sizeof(CUSTOMVERTEX));
  m_d3d_device->SetIndices(m_i_buffer);
  
  m_d3d_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 5, 0, 6);
  
  
  m_d3d_device->EndScene();
  m_d3d_device->Present(NULL, NULL, NULL, NULL);
  
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
  if (d < MAX_SEC_ON_FRAME)
  {
    d = (MAX_SEC_ON_FRAME - d) * 1000;
    Sleep((unsigned long)d);
  }
}
