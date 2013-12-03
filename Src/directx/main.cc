#include <windows.h>
#include <windowsx.h>

#include <cstdlib>
#include <ctime>

#include "app.h"

int WINAPI WinMain(HINSTANCE h_inst, HINSTANCE h_prev_inst, LPSTR lp_cmd_line, int cmd_show)
{
  srand(time(NULL));
  
  App app = App(h_inst, h_prev_inst, lp_cmd_line, cmd_show);
  
  return app.run();
}
