#pragma once

#include <gtkmm.h>

#include "left_panel.h"
#include "sky_scene.h"

class App : public Gtk::Window
{
public:
  App();
  virtual ~App();

protected:
  Gtk::HBox m_main_box;
  LeftPanel m_left_panel;
  SkyScene m_sky_scene;

protected:
  bool on_key_press_event(GdkEventKey* event);
};
