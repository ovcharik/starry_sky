#pragma once

#include <gtkmm.h>

class LeftPanel : public Gtk::VBox
{
public:
  LeftPanel();
  virtual ~LeftPanel();

protected:
  Gtk::Adjustment m_adj_count;
  sigc::connection m_adj_count_connection;
  
  Gtk::Adjustment m_adj_time;
  sigc::connection m_adj_time_connection;
  
  Gtk::Adjustment m_adj_percent_big;
  sigc::connection m_adj_percent_big_connection;
  Gtk::Adjustment m_adj_percent_middle;
  sigc::connection m_adj_percent_middle_connection;
  Gtk::Adjustment m_adj_percent_small;
  sigc::connection m_adj_percent_small_connection;

protected:
  Gtk::HScale m_scale_count;
  
  Gtk::HScale m_scale_time;
  
  Gtk::HScale m_scale_percent_big;
  Gtk::HScale m_scale_percent_middle;
  Gtk::HScale m_scale_percent_small;
  
  Gtk::Label m_fps;
};
