#pragma once

#include <functional>
#include "SDL.h"

namespace gamestate
{
  struct t
  {
    std::function<int(float dt)> update;
    std::function<int()> draw;
    std::function<int(SDL_Event &event)> treatEvent;
    std::function<int(gamestate::t &previous)> enter;
    std::function<int(gamestate::t &next, std::function<int()> on_left)> leave;
  };

  int switchTo(gamestate::t &next);
  int draw();
  int update(float dt);
  int treatEvent(SDL_Event &event);
}
