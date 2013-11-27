#pragma once

#include <gtkmm.h>
#include <gtkglmm.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <SDL/SDL.h>

#include <ctime>

#include "defines.h"
#include "stars_storage.h"
#include "cloud.h"

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
  void get_frustum(float* proj, float* modl, float frustum[6][4]);
  bool point_in_frustum(float x, float y, float z, float frustum[6][4]);
  bool rect_in_frustum(float rect[4][3], float frustum[6][4]);

protected:
  
  int m_frames;
  
  unsigned long m_clocks;
  unsigned long m_prev_clocks;
  unsigned long m_pause;
};
