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
    
    m_d3d_inited(false)
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
      this->m_view_rot_x -= DELTA_ROT / 5;
      this->render();
    });
  EventRepository::get_event<int>(EventTypes::WN_KEY_DOWN)
    ->add_handler([this](int key){
      this->m_view_rot_x += DELTA_ROT / 5;
      this->render();
    });
  EventRepository::get_event<int>(EventTypes::WN_KEY_LEFT)
    ->add_handler([this](int key){
      this->m_view_rot_y -= DELTA_ROT / 5;
      this->render();
    });
  EventRepository::get_event<int>(EventTypes::WN_KEY_RIGHT)
    ->add_handler([this](int key){
      this->m_view_rot_y += DELTA_ROT / 5;
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
  if (m_d3d_inited)
  {
    m_textures[0]->Release();
    m_textures[1]->Release();
    m_d3d_device->Release();
    m_d3d_interface->Release();
  }
}

LRESULT SkyScene::wnd_proc(HWND h_wnd, UINT msg, WPARAM w_param, LPARAM l_param)
{
  switch (msg)
  {
  }
  return DefWindowProc(h_wnd, msg, w_param, l_param);
}

void SkyScene::init()
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
  
  D3DXCreateTextureFromFile(
    m_d3d_device,
    "Data/Star.bmp",
    &(m_textures[0])
  );
  
  D3DXCreateTextureFromFile(
    m_d3d_device,
    "Data/Cloud.bmp",
    &(m_textures[1])
  );
  
  // Set states
  m_d3d_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
  m_d3d_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
  
  m_d3d_device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
  m_d3d_device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  m_d3d_device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  
  m_d3d_device->SetRenderState(D3DRS_LIGHTING, FALSE);
  m_d3d_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
  m_d3d_device->SetRenderState(D3DRS_ZENABLE, FALSE);
  
  m_d3d_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  m_d3d_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  m_d3d_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
  m_d3d_device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
  
  
  m_d3d_inited = true;
}

void SkyScene::render()
{
  // Render scene
  
  if (!m_d3d_inited)
    return;
  
  m_d3d_device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
  m_d3d_device->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
  
  m_d3d_device->BeginScene();
  
  // Set cam
  m_d3d_device->SetFVF(POSITION_COLOR_FVF);
  
  D3DXMATRIX mat_view, mat_x, mat_y, mat_z, mat_rot, mat_trans;
  D3DXMatrixRotationX(&mat_x, m_view_rot_x);
  D3DXMatrixRotationY(&mat_y, m_view_rot_y);
  D3DXMatrixRotationZ(&mat_z, m_view_rot_z);
  mat_rot = mat_x * mat_y * mat_z;
  
  D3DXMatrixTranslation(&mat_trans, 0.0f, 0.0f, m_view_pos_z);
  mat_view = mat_rot * mat_trans;
  
  m_d3d_device->SetTransform(D3DTS_VIEW, &mat_view);
  
  D3DXMATRIX mat_projection;
  D3DXMatrixPerspectiveFovLH(
    &mat_projection,
    D3DX_PI / 4,
    (float)get_width() / (float)get_height(),
    5.0f,
    4000.0f);
  m_d3d_device->SetTransform(D3DTS_PROJECTION, &mat_projection);
  
  float* modl_view = static_cast<FLOAT*>(mat_view);
  float* proj_view = static_cast<FLOAT*>(mat_projection);
  
  float frustum[6][4];
  Frustum::get_frustum(proj_view, modl_view, frustum);
  
  float billboard[4][3];
  float roted_billboard[4][3];
  
  // Create vertex buffer
  LPDIRECT3DVERTEXBUFFER9 v_buffer;
  
  std::vector<POSITION_COLOR_VERTEX> vertices;
  
  static const float tex_coords[4][2] = {
    {1, 1},
    {0, 1},
    {1, 0},
    {0, 0}
  };
  
  // Stars
  auto stars = m_stars_storage.get_stars();
  for (auto star : stars)
  {
    Position p = star.get_position();
    if (!Frustum::point_in_frustum(p.x, p.y, p.z, frustum))
      continue;
    
    Color c = star.get_color();
    float d = star.get_size(3.0f, 2.0f, 1.0f);
    star.get_billboard(modl_view, d, billboard);
    star.get_roted_billboard(billboard, roted_billboard);
    
    for (int i = 0; i < 4; i++)
    {
      POSITION_COLOR_VERTEX vertex = {
        billboard[i][0],
        billboard[i][1],
        billboard[i][2],
        D3DCOLOR_COLORVALUE(c.red, c.green, c.blue, c.alpha),
        tex_coords[i][0],
        tex_coords[i][1]
      };
      vertices.push_back(vertex);
    }
    for (int i = 0; i < 4; i++)
    {
      POSITION_COLOR_VERTEX vertex = {
        roted_billboard[i][0],
        roted_billboard[i][1],
        roted_billboard[i][2],
        D3DCOLOR_COLORVALUE(1.0, 1.0, 1.0, c.alpha),
        tex_coords[i][0],
        tex_coords[i][1]
      };
      vertices.push_back(vertex);
    }
    
  }
  
  // Render
  int s = vertices.size() * sizeof(POSITION_COLOR_VERTEX);
  
  if (s > 0)
  {
    m_d3d_device->CreateVertexBuffer(
      s,
      0,
      POSITION_COLOR_FVF,
      D3DPOOL_MANAGED,
      &v_buffer,
      NULL);
    
    VOID* pVoid;
    v_buffer->Lock(0, 0, (void**)&pVoid, 0);
    memcpy(pVoid, vertices.data(), s);
    v_buffer->Unlock();
    
    m_d3d_device->SetTexture(0, m_textures[0]);
    m_d3d_device->SetStreamSource(0, v_buffer, 0, sizeof(POSITION_COLOR_VERTEX));
    for (int i = 0; i < vertices.size(); i += 4) {
      m_d3d_device->DrawPrimitive(D3DPT_TRIANGLESTRIP, i, 2);
    }
    
    v_buffer->Release();
  }
  
  vertices.clear();
  
  // Clouds
  float tex_points[4][2];
  auto particals = m_cloud.get_particles();
  for (auto part : particals)
  {
    part.get_billboard(modl_view, billboard);
    if (!Frustum::rect_in_frustum(billboard, frustum))
      continue;
    
    part.get_texture(4, 4, tex_points);
    
    for (int i = 0; i < 4; i++)
    {
      POSITION_COLOR_VERTEX vertex = {
        billboard[i][0],
        billboard[i][1],
        billboard[i][2],
        D3DCOLOR_COLORVALUE(1.0, 0.7, 1.0, 0.1),
        tex_points[i][0],
        tex_points[i][1]
      };
      vertices.push_back(vertex);
    }
  }
  
  // Render
  s = vertices.size() * sizeof(POSITION_COLOR_VERTEX);
  
  if (s > 0)
  {
    m_d3d_device->CreateVertexBuffer(
      s,
      1,
      POSITION_COLOR_FVF,
      D3DPOOL_MANAGED,
      &v_buffer,
      NULL);
    
    VOID* pVoid;
    v_buffer->Lock(0, 0, (void**)&pVoid, 0);
    memcpy(pVoid, vertices.data(), s);
    v_buffer->Unlock();
    
    m_d3d_device->SetTexture(0, m_textures[1]);
    m_d3d_device->SetStreamSource(0, v_buffer, 0, sizeof(POSITION_COLOR_VERTEX));
    for (int i = 0; i < vertices.size(); i += 4) {
      m_d3d_device->DrawPrimitive(D3DPT_TRIANGLESTRIP, i, 2);
    }
    
    v_buffer->Release();
  }
  
  // End render
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
