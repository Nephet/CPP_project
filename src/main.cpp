//! --------------------------------------------------------------------------
//! -------------------------- INCLUDES
//! --------------------------------------------------------------------------

#include <cstdlib>
#include <ctime>
#include <functional>
#include <SDL.h>

#include "debug/assert.h"

#include "graphics/opengl.h"
#include "graphics/Texture.hpp"

#include "math/wjd_math.h"

#include "global.hpp"

//! --------------------------------------------------------------------------
//! -------------------------- NAMESPACES
//! --------------------------------------------------------------------------

using namespace std;

//! --------------------------------------------------------------------------
//! -------------------------- CONSTANTS
//! --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// EVENTS
// --------------------------------------------------------------------------

#define EVENT_QUIT 0b00000001

//! --------------------------------------------------------------------------
//! -------------------------- WORKSPACE
//! --------------------------------------------------------------------------

static SDL_Window *window = nullptr;

//! --------------------------------------------------------------------------
//! -------------------------- GAME STATES
//! --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// GAMESTATE STRUCTURE
// --------------------------------------------------------------------------

struct gamestate_t
{
  function<int(float dt)> update;
  function<int()> draw;
  function<int(SDL_Event &event)> treatEvent;
  function<int(gamestate_t &previous)> enter;
  function<int(gamestate_t &next)> leave;
};

static gamestate_t title, ingame, &current_state = title;

static float t = 0.0f;
static float entering = 0.0f;
static float exiting = -1.0f;
static fRect sprite(0, 0, 256, 256);

static Texture texture;

int createStates()
{
  // --------------------------------------------------------------------------
  // TITLE GAMESTATE
  // --------------------------------------------------------------------------

  {

  title.update = [](float dt)
  {
    // EXIT HAS STARTED
    if(exiting >= 0)
    {
      exiting += dt;

      float p = exiting*exiting; // quadratic

      float wheel = sin(PI*2*t);

      float s = (196 + 64*wheel)*(1.0f - p);

      sprite.x = global::viewport.x * (0.5f + 0.5f * p) + s*0.5f*p - s*0.5f;
      sprite.y = global::viewport.y * 0.5f - s*0.5f;
      sprite.w = sprite.h = s;

      if(exiting > 1)
        return EVENT_QUIT;
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
      t += dt;
      if(t > 1)
        t -= 1;
      float wheel = sin(PI*2*t);

      float s = 196 + 64*wheel;
      sprite.x = global::viewport.x * 0.5f - s*0.5f;
      sprite.y = global::viewport.y * 0.5f - s*0.5 + 0.2f*s*wheel;
      sprite.h = sprite.w = s;
    }

    // All okay
    return 0;
  };

  title.draw = []()
  {
    // Only draw if enter has begun
    if(entering > 0 && exiting < 1)
    {
      texture.draw(nullptr, &sprite);
    }

    // All okay
    return 0;
  };

  title.treatEvent = [](SDL_Event &event)
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

  title.leave = [](gamestate_t &next)
  {
    log("Leaving title");

    // Unload all the assets we used
    texture.unload();

    // All okay
    return 0;
  };

  title.enter = [](gamestate_t &previous)
  {
    log("Entering title");

    // Load all the assets we need
    ASSERT(texture.load("assets/eye_of_draining.png")
           == EXIT_SUCCESS, "Opening texture");
    // All okay
    return 0;
  };

  } // title scope


  // Initialise initial state
  current_state.enter(current_state);

  // All clear
  return EXIT_SUCCESS;
}

//! --------------------------------------------------------------------------
//! -------------------------- GAME LOOP
//! --------------------------------------------------------------------------

int update(float dt)
{
  // Cap delta-time
  if(dt > MAX_DT)
    dt = MAX_DT;

  // Update, accumulate event flags
  int flags = title.update(dt);

  // Treat input events
  static SDL_Event event;

  // Write each event to our static variable
  while (SDL_PollEvent(&event))
    flags |= title.treatEvent(event);

  // Returns flags
  return flags;
}

int draw()
{
  // Clear and reset
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);

  // Draw the current state
  title.draw();

  // Flip the buffers to update the screen
  SDL_GL_SwapWindow(window);

  // All good
  return EXIT_SUCCESS;
}

//! --------------------------------------------------------------------------
//! -------------------------- STARTUP
//! --------------------------------------------------------------------------

// Main must have exactly this signature or SDL2 will be sad
int main(int argc, char *argv[])
{
  // Initialise random numbers
  srand(time(NULL));

  // --------------------------------------------------------------------------
  // START SDL
  // --------------------------------------------------------------------------

  // Set up SDL (create window and context for OpenGL)
	window = SDL_CreateWindow(APP_NAME, SDL_WINDOWPOS_UNDEFINED,
                           SDL_WINDOWPOS_UNDEFINED, WINDOW_DEFAULT_W,
                           WINDOW_DEFAULT_H, SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN);
  ASSERT_SDL(window, "Opening SDL2.0 application window");

  // Since the window size can be overriden, check what it is actually
  SDL_GetWindowSize(window, &global::viewport.x, &global::viewport.y);
  global::scale.x = global::scale.y = 1.0f;
  global::viewport.x = WINDOW_DEFAULT_W;
  global::viewport.y = WINDOW_DEFAULT_H;

  // Create the OpenGL context for the window we just opened
  auto context = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, context);

  // Configure SDL/OpenGL interface
  ASSERT_SDL(SDL_GL_SetSwapInterval(1) != -1, "Activating SDL V-sync");
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, GL_V_MAJOR);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, GL_V_MINOR);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  // --------------------------------------------------------------------------
  // START OPENGL
  // --------------------------------------------------------------------------

  {

  // Define viewport
  glViewport(0, 0, global::viewport.x, global::viewport.y);

  // Black background by default
  glClearColor(0, 0, 0, 255);

  // Texturing
  glEnable(GL_TEXTURE_2D);

  // Blending and anti-aliasing
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

  // Disable depth-testing
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

	// Disable lighting
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHT1);

  // Set up viewport
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, WINDOW_DEFAULT_W, WINDOW_DEFAULT_H, 0, -1, 1);

  // Clean the slate
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  } // start opengl

  // --------------------------------------------------------------------------
  // CREATE GAMESTATES
  // --------------------------------------------------------------------------

  ASSERT(createStates() == EXIT_SUCCESS, "Creating states");

  // --------------------------------------------------------------------------
  // START THE GAME LOOP
  // --------------------------------------------------------------------------

  {

  float prev_tick, this_tick = SDL_GetTicks();
  bool stop = false;
  do
  {
    // Get the current time-stamp
    prev_tick = this_tick;
    this_tick = SDL_GetTicks();

    // Update, check for exit events
    stop = (update((this_tick - prev_tick)/1000.0f) & EVENT_QUIT);

    // Redraw everything, game objects included
    draw();
  }
  while(!stop);

  } // game loop

  // --------------------------------------------------------------------------
  // SHUT DOWN
  // --------------------------------------------------------------------------

  // Destroy context
  SDL_GL_MakeCurrent(NULL, NULL);
  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);

  // Shut down SDL
	SDL_Quit();

  // Report a clean exit!
	return EXIT_SUCCESS;
}
