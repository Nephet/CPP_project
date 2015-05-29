#include "title.h"

#include "ingame.h"

#include "../math/wjd_math.h"
#include "../math/Rect.hpp"
#include "../graphics/Texture.hpp"
#include "../global.hpp"

#include "../debug/assert.h"

static float time;
static float entering;
static float exiting;
static fRect sprite(0, 0, 256, 256);
static Texture texture;
static bool toInGame;

using namespace std;

namespace gamestate
{


static gamestate::t state;

static bool initialised = false;

static int init()
{
  state.update = [](float dt)
  {
    // EXIT HAS STARTED
    if(exiting >= 0)
    {
      exiting += dt;

      float p = exiting*exiting; // quadratic

      float wheel = sin(PI*2*time);

      float s = (196 + 64*wheel)*(1.0f - p);

      sprite.x = global::viewport.x * (0.5f + 0.5f * p) + s*0.5f*p - s*0.5f;
      sprite.y = global::viewport.y * 0.5f - s*0.5f;
      sprite.w = sprite.h = s;

      if(exiting > 1)
      {
        if(toInGame)
          return gamestate::switchTo(ingame::get());
        else
          return EVENT_QUIT;
      }

    }

    // ENTER HAS STARTED
    else if(entering >= 0 && entering < 1)
    {
      entering += dt;

      float p = entering*entering; // quadratic

      float s = 256*p;
      sprite.w = sprite.h = s;

      sprite.x = global::viewport.x * 0.5f * p - s*0.5f;
      sprite.y = global::viewport.y * 0.5f - s*0.5f;
      sprite.w = sprite.h = s;

      if(entering > 1)
        entering = 1;
    }

    // ENTER HAS FINISHED
    else
    {
      time += dt;
      if(time > 1)
        time -= 1;
      float wheel = sin(PI*2*time);

      float s = 196 + 64*wheel;
      sprite.x = global::viewport.x * 0.5f - s*0.5f;
      sprite.y = global::viewport.y * 0.5f - s*0.5 + 0.2f*s*wheel;
      sprite.h = sprite.w = s;
    }

    // All okay
    return 0;
  };

  state.draw = []()
  {
    // Only draw if enter has begun
    if(entering > 0 && exiting < 1)
    {
      texture.draw(nullptr, &sprite);
    }

    // All okay
    return 0;
  };

  state.treatEvent = [](SDL_Event &event)
  {
    switch (event.type)
    {
      // Exit if the window is closed
      // (ex: pressing the cross at the top)
      case SDL_QUIT:
        return EVENT_QUIT;
      break;

      // Check for key-presses
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym)
        {
          case SDLK_RETURN:
            if(entering >= 1 && exiting < 0)
            {
              exiting = 0;
              toInGame = true;
            }
          break;

          case SDLK_ESCAPE:
            if(entering >= 1 && exiting < 0)
              exiting = 0;
          default:
            break;
        }
      break;

      default:
        // not all possible inputs are needed, so we DO want a default break
      break;
    }


    // All okay
    return 0;
  };

  state.leave = [](gamestate::t &next, function<int()> onleft)
  {
    log("Leaving title");

    // Unload all the assets we used
    texture.unload();

    // All okay
    return onleft();
  };

  state.enter = [](gamestate::t &previous)
  {
    log("Entering title");

    // Reset state
    time = 0.0f;
    entering = 0.0f;
    exiting = -1.0f;
    toInGame = false;

    // Load all the assets we need
    ASSERT(texture.load("assets/eye_of_draining.png")
           == EXIT_SUCCESS, "Opening texture");
    // All okay
    return 0;
  };

  // Init success
  return 0;
}

namespace title
{

  gamestate::t &get()
  {
    if(!initialised)
    {
      init();
      initialised = true;
    }

    return state;
  }

} // title

} // gamestate
