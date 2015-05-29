#include "ingame.h"

#include "title.h"

#include "../math/wjd_math.h"
#include "../math/Rect.hpp"
#include "../graphics/Texture.hpp"
#include "../global.hpp"

#include "../debug/assert.h"

#define GRID_W 30
#define GRID_H 30
#define TILE_LAVA 1
#define TILE_ICE 2

static Texture atlas, spaceship;
static int tiles[GRID_W][GRID_H];
static fRect sprite(0, 0, 180, 180),
  lava(0, 0, 32, 32),
  ice(32, 0, 32, 32);

static float angle = 0.0f;

using namespace std;

namespace gamestate
{

static gamestate::t state;

static bool initialised = false;

static int init()
{
  state.update = [](float dt)
  {
    // Spin the ship
    angle += 360*dt;

    // Centre the ship
    sprite.x = (global::viewport.x - sprite.w) * 0.5f;
    sprite.y = (global::viewport.y - sprite.h) * 0.5f;

    // All okay
    return 0;
  };

  state.draw = []()
  {

    // Draw the grid
    for(int x = 0; x < GRID_W; x++)
    for(int y = 0; y < GRID_H; y++)
    {
      static fRect stamp(0, 0, 40, 40);
      stamp.x = stamp.w*x;
      stamp.y = stamp.h*y;
      switch(tiles[x][y])
      {
        case TILE_ICE:
          atlas.draw(&ice, &stamp);
          break;

        case TILE_LAVA:
          atlas.draw(&lava, &stamp);
          break;
      }
    }

    // Draw the ship
    spaceship.draw(nullptr, &sprite, angle);

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
          break;

          case SDLK_ESCAPE:
            return gamestate::switchTo(title::get());
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
    log("Leaving ingame");

    // Unload all the assets we used
    atlas.unload();
    spaceship.unload();

    // Leave instantly
    return onleft();
  };

  state.enter = [](gamestate::t &previous)
  {
    log("Entering ingame");

    // Reset state
    for(int x = 0; x < GRID_W; x++)
    for(int y = 0; y < GRID_H; y++)
    {
      tiles[x][y] = (rand()%2 ? TILE_ICE : TILE_LAVA);
    }

    // Load all the assets we need
    ASSERT(atlas.load("assets/atlas.png") == EXIT_SUCCESS, "Opening atlas texture");
    ASSERT(spaceship.load("assets/medspeedster.png") == EXIT_SUCCESS, "Opening spaceship texture");

    // All okay
    return 0;
  };

  // Init success
  return 0;
}

namespace ingame
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

} // ingame

} // gamestate
