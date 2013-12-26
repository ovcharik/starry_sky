#pragma once

#include <gtkmm.h>
#include <gtkglmm.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <SDL/SDL.h>

#include <ctime>

#include "../general/defines.h"
#include "../general/stars_storage.h"
#include "../general/cloud.h"

class SkyScene : public Gtk::DrawingArea,
                 public Gtk::GL::Widget<SkyScene>
{
public:
  SkyScene();
  virtual ~SkyScene();
  
protected:
  virtual void on_realize();
  virtual bool on_configure_event(GdkEventConfigure* event);
  virtual bool on_expose_event(GdkEventExpose* event);
  
  virtual bool on_map_event(GdkEventAny* event);
  virtual bool on_unmap_event(GdkEventAny* event);
  virtual bool on_visibility_notify_event(GdkEventVisibility* event);
  virtual bool on_idle();
  
  sigc::connection m_connection_idle;

public:
  void invalidate();
  void update();

protected:
  GLfloat m_view_rot_x;
  GLfloat m_view_rot_y;
  GLfloat m_view_rot_z;
  
  GLfloat m_view_pos_z;
  
  GLuint m_textures[2];
  
  StarsStorage m_stars_storage;
  Cloud m_cloud;

protected:
  
  int m_frames;
  
  unsigned long m_clocks;
  unsigned long m_prev_clocks;
  
  bool m_paused;
};
