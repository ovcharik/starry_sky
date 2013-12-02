#include <iostream>
#include <cstdlib>

#include "sky_scene.h"

#include "../general/event_repository.h"

SkyScene::SkyScene()
  : m_view_rot_x(DEFAULT_ROT_X),
    m_view_rot_y(DEFAULT_ROT_Y),
    m_view_rot_z(DEFAULT_ROT_Z),
    
    m_view_pos_z(DEFAULT_POS_Z),
    
    m_cloud(Position(0, 0, 350), 200, 100, 100, 100),
    
    m_frames(0),
    m_clocks(SDL_GetTicks()),
    m_prev_clocks(SDL_GetTicks()),
    m_pause(0)
{
  Glib::RefPtr<Gdk::GL::Config> glconfig;
  glconfig = Gdk::GL::Config::create(Gdk::GL::MODE_RGB    |
                                     Gdk::GL::MODE_DEPTH  |
                                     Gdk::GL::MODE_SINGLE);
  if (!glconfig)
  {
    std::cerr << "*** Cannot find any OpenGL-capable visual.\n";
    std::exit(1);
  }
  set_gl_capability(glconfig);
  
  // set events
  EventRepository::get_event<GdkEventKey*>(EventTypes::WN_KEY_UP)
    ->add_handler([this](GdkEventKey* event){
      this->m_view_rot_x -= DELTA_ROT;
      this->invalidate();
    });
  EventRepository::get_event<GdkEventKey*>(EventTypes::WN_KEY_DOWN)
    ->add_handler([this](GdkEventKey* event){
      this->m_view_rot_x += DELTA_ROT;
      this->invalidate();
    });
  EventRepository::get_event<GdkEventKey*>(EventTypes::WN_KEY_LEFT)
    ->add_handler([this](GdkEventKey* event){
      this->m_view_rot_y -= DELTA_ROT;
      this->invalidate();
    });
  EventRepository::get_event<GdkEventKey*>(EventTypes::WN_KEY_RIGHT)
    ->add_handler([this](GdkEventKey* event){
      this->m_view_rot_y += DELTA_ROT;
      this->invalidate();
    });
  
  EventRepository::get_event<GdkEventKey*>(EventTypes::WN_KEY_PAGE_UP)
    ->add_handler([this](GdkEventKey* event){
      this->m_view_pos_z += DELTA_POS;
      this->invalidate();
    });
  EventRepository::get_event<GdkEventKey*>(EventTypes::WN_KEY_PAGE_DOWN)
    ->add_handler([this](GdkEventKey* event){
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

// -------------
void SkyScene::invalidate()
{
  get_window()->invalidate_rect(get_allocation(), false);
}

void SkyScene::update()
{
  get_window()->process_updates(false);
}

// -------------
void SkyScene::on_realize()
{
  Gtk::DrawingArea::on_realize();
  Glib::RefPtr<Gdk::GL::Window> glwindow = get_gl_window();
  if (!glwindow->gl_begin(get_gl_context()))
    return;
  
  glEnable(GL_CULL_FACE);
  glEnable(GL_LIGHT0);
  
  glEnable(GL_TEXTURE_2D);
  glShadeModel(GL_SMOOTH);
  
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  glEnable(GL_BLEND);
  glEnable(GL_ALPHA_TEST);
  
  SDL_Surface* textures[2];
  textures[0] = SDL_LoadBMP("Data/Star.bmp");
  textures[1] = SDL_LoadBMP("Data/Cloud.bmp");
  
  glGenTextures(2, &m_textures[0]);
  for (int i = 0; i < 2; i++)
  {
    glBindTexture(GL_TEXTURE_2D, m_textures[i]);
  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  
    glTexImage2D(
      GL_TEXTURE_2D, 0, 3,
      textures[i]->w, textures[i]->h,
      0, GL_RGB, GL_UNSIGNED_BYTE,
      textures[i]->pixels);
    
    if (textures[i])
      SDL_FreeSurface(textures[i]);
  }
  
  glEnable(GL_NORMALIZE);
  
  glwindow->gl_end();
}

bool SkyScene::on_configure_event(GdkEventConfigure* event)
{
  Glib::RefPtr<Gdk::GL::Window> glwindow = get_gl_window();
  if (!glwindow->gl_begin(get_gl_context()))
    return false;
  
  GLfloat h = (GLfloat)(get_height()) / (GLfloat)(get_width());
  glViewport(0, 0, get_width(), get_height());
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-1.0f, 1.0f, -h, h, 5.0f, 4000.0f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  glwindow->gl_end();
  return true;
}

bool SkyScene::on_expose_event(GdkEventExpose* event)
{
  Glib::RefPtr<Gdk::GL::Window> glwindow = get_gl_window();
  if (!glwindow->gl_begin(get_gl_context()))
    return false;
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glLoadIdentity();
  glTranslatef(0.0f, 0.0f, m_view_pos_z);

  glRotatef(m_view_rot_x, 1.0, 0.0, 0.0);
  glRotatef(m_view_rot_y, 0.0, 1.0, 0.0);
  glRotatef(m_view_rot_z, 0.0, 0.0, 1.0);
  
  float proj_view[16];
  float modl_view[16];
  glGetFloatv(GL_PROJECTION_MATRIX, proj_view);
  glGetFloatv(GL_MODELVIEW_MATRIX, modl_view);
  
  float frustum[6][4];
  get_frustum(proj_view, modl_view, frustum);
  
  float billboard[4][3];
  float roted_billboard[4][3];
  
  glBindTexture(GL_TEXTURE_2D, m_textures[0]);
  auto stars = m_stars_storage.get_stars();
  for (auto star : stars)
  {
    Position p = star.get_position();
    if (!point_in_frustum(p.x, p.y, p.z, frustum))
      continue;
    
    Color c = star.get_color();
    float d = star.get_size(3.0f, 2.0f, 1.0f);
    
    star.get_billboard(modl_view, d, billboard);
    star.get_roted_billboard(billboard, roted_billboard);
    
    glColor4f(1.0, 1.0, 1.0, c.alpha);
    glBegin(GL_TRIANGLE_STRIP);
      glTexCoord2d(1, 1); glVertex3fv(roted_billboard[0]);
      glTexCoord2d(0, 1); glVertex3fv(roted_billboard[1]);
      glTexCoord2d(1, 0); glVertex3fv(roted_billboard[2]);
      glTexCoord2d(0, 0); glVertex3fv(roted_billboard[3]);
    glEnd();
    
    glColor4f(c.red, c.green, c.blue, c.alpha);
    glBegin(GL_TRIANGLE_STRIP);
      glTexCoord2d(1, 1); glVertex3fv(billboard[0]);
      glTexCoord2d(0, 1); glVertex3fv(billboard[1]);
      glTexCoord2d(1, 0); glVertex3fv(billboard[2]);
      glTexCoord2d(0, 0); glVertex3fv(billboard[3]);
    glEnd();
  }
  
  float tex_points[4][2];
  glBindTexture(GL_TEXTURE_2D, m_textures[1]);
  glColor4f(1.0, 0.7, 1.0, 0.1);
  auto particals = m_cloud.get_particles();
  for (auto part : particals)
  {
    part.get_billboard(modl_view, billboard);
    if (!rect_in_frustum(billboard, frustum))
      continue;
    
    part.get_texture(4, 4, tex_points);
    
    glBegin(GL_TRIANGLE_STRIP);
      for (int i = 0; i < 4; i++)
      {
        glTexCoord2d(tex_points[i][0], tex_points[i][1]);
        glVertex3fv(billboard[i]);
      }
    glEnd();
  }
  
  
  if (glwindow->is_double_buffered())
    glwindow->swap_buffers();
  else
    glFlush();
  
  glwindow->gl_end();
  
  m_frames++;
  clock_t t = SDL_GetTicks();
  
  double seconds = (t - m_clocks) / 1000.0;
  if (seconds > 0.5)
  {
    double fps = m_frames / seconds;
    m_frames = 0;
    m_clocks = t;
    
    EventRepository::get_event<int>(EventTypes::SS_TIMER)->trigger(0);
    EventRepository::get_event<double>(EventTypes::SS_FPS_CHANGED)->trigger(fps);
  }
  
  // limiting fps
  double d = (t - m_prev_clocks) / 1000.0;
  m_prev_clocks = t;
  if (d < MAX_SEC_ON_FRAME)
  {
    d = (MAX_SEC_ON_FRAME - d) * 1000000;
    usleep((unsigned long)d);
  }
  
  return true;
}

// ----------------
bool SkyScene::on_map_event(GdkEventAny* event)
{
  if (!m_connection_idle.connected())
    m_connection_idle = Glib::signal_idle().connect(
      sigc::mem_fun(*this, &SkyScene::on_idle), GDK_PRIORITY_REDRAW);

  return true;
}

bool SkyScene::on_unmap_event(GdkEventAny* event)
{
  if (m_connection_idle.connected())
    m_connection_idle.disconnect();

  return true;
}

bool SkyScene::on_visibility_notify_event(GdkEventVisibility* event)
{
  if (event->state == GDK_VISIBILITY_FULLY_OBSCURED)
    {
      if (m_connection_idle.connected())
        m_connection_idle.disconnect();
    }
  else
    {
      if (!m_connection_idle.connected())
        m_connection_idle = Glib::signal_idle().connect(
          sigc::mem_fun(*this, &SkyScene::on_idle), GDK_PRIORITY_REDRAW);
    }

  return true;
}

bool SkyScene::on_idle()
{
  invalidate();
  update();
  return true;
}

// -------------------
void SkyScene::get_frustum(float* proj, float* modl, float frustum[6][4])
{
  float   clip[16];

  clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8] + modl[ 3] * proj[12];
  clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9] + modl[ 3] * proj[13];
  clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10] + modl[ 3] * proj[14];
  clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11] + modl[ 3] * proj[15];

  clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4] + modl[ 6] * proj[ 8] + modl[ 7] * proj[12];
  clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9] + modl[ 7] * proj[13];
  clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10] + modl[ 7] * proj[14];
  clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11] + modl[ 7] * proj[15];

  clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4] + modl[10] * proj[ 8] + modl[11] * proj[12];
  clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9] + modl[11] * proj[13];
  clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10] + modl[11] * proj[14];
  clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11] + modl[11] * proj[15];

  clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4] + modl[14] * proj[ 8] + modl[15] * proj[12];
  clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] + modl[15] * proj[13];
  clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] + modl[15] * proj[14];
  clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] + modl[15] * proj[15];

  /* ПРАВАЯ плоскость */
  frustum[0][0] = clip[ 3] - clip[ 0];
  frustum[0][1] = clip[ 7] - clip[ 4];
  frustum[0][2] = clip[11] - clip[ 8];
  frustum[0][3] = clip[15] - clip[12];
  /* ЛЕВАЯ плоскость */
  frustum[1][0] = clip[ 3] + clip[ 0];
  frustum[1][1] = clip[ 7] + clip[ 4];
  frustum[1][2] = clip[11] + clip[ 8];
  frustum[1][3] = clip[15] + clip[12];

  /* НИЖНЯЯ плоскость */
  frustum[2][0] = clip[ 3] + clip[ 1];
  frustum[2][1] = clip[ 7] + clip[ 5];
  frustum[2][2] = clip[11] + clip[ 9];
  frustum[2][3] = clip[15] + clip[13];

  /* ВЕРХНЯЯ плоскость */
  frustum[3][0] = clip[ 3] - clip[ 1];
  frustum[3][1] = clip[ 7] - clip[ 5];
  frustum[3][2] = clip[11] - clip[ 9];
  frustum[3][3] = clip[15] - clip[13];

  /* ЗАДНЯЯ плоскость */
  frustum[4][0] = clip[ 3] - clip[ 2];
  frustum[4][1] = clip[ 7] - clip[ 6];
  frustum[4][2] = clip[11] - clip[10];
  frustum[4][3] = clip[15] - clip[14];

  /* ПЕРЕДНЯЯ плоскость */
  frustum[5][0] = clip[ 3] + clip[ 2];
  frustum[5][1] = clip[ 7] + clip[ 6];
  frustum[5][2] = clip[11] + clip[10];
  frustum[5][3] = clip[15] + clip[14];
  
  for (int i = 0; i < 6; i++)
  {
    float t = sqrt(frustum[i][0] * frustum[i][0] + frustum[i][1] * frustum[i][1] + frustum[i][2] * frustum[i][2]);
    for (int j = 0; j < 4; j++)
      frustum[i][j] /= t;
  }
}

bool SkyScene::point_in_frustum(float x, float y, float z, float frustum[6][4])
{
  for(int p = 0; p < 6; p++)
    if( frustum[p][0] * x + frustum[p][1] * y + frustum[p][2] * z + frustum[p][3] <= 0 )
      return false;
  return true;
}

bool SkyScene::rect_in_frustum(float rect[4][3], float frustum[6][4])
{
  for (int i = 0; i < 4; i++)
    if (point_in_frustum(rect[i][0], rect[i][1], rect[i][2], frustum))
      return true;
  return false;
}
