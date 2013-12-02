#include <ctime>
#include <cstdlib>

#include <gtkmm.h>
#include <gtkglmm.h>

#include "app.h"

int main(int argc, char *argv[])
{
  srand(time(NULL));
  Gtk::Main kit(argc, argv);
  Gtk::GL::init(argc, argv);
  
  App app;
  kit.run(app);
  
  return 0;
}
