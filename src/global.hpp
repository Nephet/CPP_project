#pragma once

#include "math/Rect.hpp"

#define WINDOW_DEFAULT_W 800    // size used by Desktop version
#define WINDOW_DEFAULT_H 600
#define MAX_FPS 60
#define MIN_FPS 20
#define MAX_DT 1.0f/MIN_FPS
#define APP_NAME "Fibonacci Simulator 2014"

#define EVENT_QUIT 0b00000001

namespace global
{
  extern iV2 viewport;
  extern fV2 scale;
  extern float mouse_x;
  extern float mouse_y;
};

