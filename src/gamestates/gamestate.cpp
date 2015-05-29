#include "gamestate.h"

#include <functional>

using namespace std;

static gamestate::t *current = nullptr;

namespace gamestate
{
  int switchTo(gamestate::t &next)
  {
    if(current)
    {
      current->leave(next, [&next]() {
        next.enter(*current);
        current = &next;

        // Success
        return 0;
      });
    }
    else
    {
      next.enter(next);
      current = &next;
    }

    // Success
    return 0;
  }

  int draw()
  {
    current->draw();

    // Success
    return 0;
  }

  int update(float dt)
  {
    return current->update(dt);
  }

  int treatEvent(SDL_Event &event)
  {
    return current->treatEvent(event);
  }
}
