#include "app.h"
#include "defines.h"
#include "event_repository.h"

#include <iostream>

App::App()
  : m_main_box(),
    m_left_panel(),
    m_sky_scene()
{
  set_title(APP_NAME);
  set_default_size(820, 510);
  set_reallocate_redraws(true);
  
  add(m_main_box);
  
  m_main_box.set_border_width(5);
  m_main_box.set_spacing(10);
  m_main_box.pack_start(m_left_panel, Gtk::PACK_SHRINK, 0);
  
  m_main_box.pack_start(m_sky_scene);
  
  show_all();
}

App::~App()
{}

bool App::on_key_press_event(GdkEventKey* event)
{
  switch(event->keyval)
  {
  case GDK_Up:
    EventRepository::get_event<GdkEventKey*>(EventTypes::WN_KEY_UP)
      ->trigger(event);
    break;
  case GDK_Down:
    EventRepository::get_event<GdkEventKey*>(EventTypes::WN_KEY_DOWN)
      ->trigger(event);
    break;
  case GDK_Left:
    EventRepository::get_event<GdkEventKey*>(EventTypes::WN_KEY_LEFT)
      ->trigger(event);
    break;
  case GDK_Right:
    EventRepository::get_event<GdkEventKey*>(EventTypes::WN_KEY_RIGHT)
      ->trigger(event);
    break;
  case GDK_Page_Up:
    EventRepository::get_event<GdkEventKey*>(EventTypes::WN_KEY_PAGE_UP)
      ->trigger(event);
    break;
  case GDK_Page_Down:
    EventRepository::get_event<GdkEventKey*>(EventTypes::WN_KEY_PAGE_DOWN)
      ->trigger(event);
    break;
  }
  return true;
}
