//! --------------------------------------------------------------------------
//! -------------------------- INCLUDES
//! --------------------------------------------------------------------------

#include <cstdlib>
#include <SDL.h>
#include <ctime>

#include "debug/assert.h"

#include "graphics/opengl.h"
#include "graphics/Texture.hpp"

#include "math/wjd_math.h"

#include "global.hpp"

//! --------------------------------------------------------------------------
//! -------------------------- CONSTANTS
//! --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// GRID
// --------------------------------------------------------------------------

#define GRID_W 300
#define GRID_H 300
#define TILE_LAVA 1
#define TILE_ICE 2

// --------------------------------------------------------------------------
// EVENTS
// --------------------------------------------------------------------------

#define EVENT_QUIT 0b00000001

//! --------------------------------------------------------------------------
//! -------------------------- WORKSPACE
//! --------------------------------------------------------------------------

static SDL_Window *window;

static Texture atlas;

static int tiles[GRID_W][GRID_H];

static fRect sprite(0, 0, 4, 4),
  lava(0, 0, 32, 32),
  ice(32, 0, 32, 32);

//! --------------------------------------------------------------------------
//! -------------------------- GAME LOOP
//! --------------------------------------------------------------------------

int treatEvents()
{
  // Static to avoid reallocating it ever time we run the function
  static SDL_Event event;

  // Write each APP_NAMEevent to our static variable
  while (SDL_PollEvent(&event))
  {
    switch (event.type)
    {
      // Exit if the window is closed (ex: pressing the cross at the top)
      case SDL_QUIT:
        return EVENT_QUIT;
      break;

      // Check for key-presses
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym)
        {
          case SDLK_ESCAPE:
            return EVENT_QUIT;
          default:
            break;
        }
      break;

      default:
        // not all possible inputs are needed, so we DO want a default break
      break;
    }
  }

  // No event
  return 0;
}

int update(float dt)
{
  log("dt = %f", dt);

  // Cap delta-time
  if(dt > MAX_DT)
    dt = MAX_DT;

  // Treat input events
  return treatEvents();
}

int draw()
{
  // Clear and reset
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);

  // Draw the grid
  for(int x = 0; x < GRID_W; x++)
  for(int y = 0; y < GRID_H; y++)
  {
    sprite.x = sprite.w*x;
    sprite.y = sprite.h*y;
    switch(tiles[x][y])
    {
      case TILE_ICE:
        atlas.draw(&ice, &sprite);
        break;

      case TILE_LAVA:
        atlas.draw(&lava, &sprite);
        break;
    }
  }

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
  glViewport(0, 0, WINDOW_DEFAULT_W, WINDOW_DEFAULT_H);

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
  // LOAD THE IMAGES
  // --------------------------------------------------------------------------

  ASSERT(atlas.load("assets/atlas.png") == EXIT_SUCCESS, "Opening atlas texture");

  // --------------------------------------------------------------------------
  // INITIALISE THE GRID
  // --------------------------------------------------------------------------

  for(int x = 0; x < GRID_W; x++)
  for(int y = 0; y < GRID_H; y++)
  {
    tiles[x][y] = (rand()%2 ? TILE_ICE : TILE_LAVA);
  }

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
