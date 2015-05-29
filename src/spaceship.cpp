#include "spaceship.h"

#include "debug/assert.h"
#include "math/Rect.hpp"
#include "graphics/Texture.hpp"

#include "global.hpp"

#include <list>

using namespace std;

static Texture texture;

static fRect sprite(0, 0, 128, 128);

struct spaceship_t
{
  float x;
  float y;
  float dx;
  float dy;
  bool update_on_even;
};

static bool event_frame = true;
static list<spaceship_t*> instances;

static float rot_c = cos(PI*0.01);
static float rot_s = sin(PI*0.01);

static bool skip = true;
static bool frame_impair= true;

namespace spaceship
{

  int update(float dt)
  {
    event_frame = !event_frame;
    for(auto i = instances.begin(); i != instances.end(); i++)
    {

      skip = !skip;

      if((skip && event_frame) || (!skip && !event_frame))
        continue;

      spaceship_t &s = (**i);

      // Turn the ship
      float vx = global::mouse_x - s.x;
      float vy = global::mouse_y - s.y;

      float norm_v = sqrt(vx*vx + vy*vy);
      float nvx = vx/norm_v;
      float nvy = vy/norm_v;

      // Do we need to turn
      float dot = s.dx*nvx + s.dy*nvy;
      if(dot < 0.99f)
      {
        // Which direction shall we turn in?
        float det = s.dx*vy - s.dy*vx;
        float rot_ss = det < 0 ? -rot_s : rot_s;

        s.dx = s.dx*rot_c - s.dy*rot_ss;
        s.dy = s.dx*rot_ss + s.dy*rot_c;

        // Normally we shouldn't need to do this!!!
        float norm = sqrt(s.dx*s.dx + s.dy*s.dy);
        s.dx /= norm;
        s.dy /= norm;
      }

      // Move the spaceship towards its destination
      if(norm_v > 16)
      {
        s.x += 256*s.dx*dt;
        s.y += 256*s.dy*dt;
      }

    }

     // Success
     return EXIT_SUCCESS;
  }


  int draw()
  {
    for(auto i = instances.begin(); i != instances.end(); i++)
    {
      spaceship_t &s = (**i);

      // Draw the sprite
      sprite.x = s.x - sprite.w*0.5f;
      sprite.y = s.y - sprite.h*0.5f;
      texture.draw(nullptr, &sprite);

      // Debug
      float x = s.x + s.dx*128;
      float y = s.y + s.dy*128;
      fRect little(0, 0, 16, 16);
      /*for(float t = 0.0f; t < 1.0f; t += 0.1f)
      {
        // Draw the spaceship's direction
        glColor4f(1.0f, 0.0f, 0.0f, 1);
        little.x = (1 - t)*s.x + t*x - 8;
        little.y = (1 - t)*s.y + t*y - 8;
        texture.draw(nullptr, &little);

        // Draw the mouse direction
        glColor4f(0.0f, 0.0f, 1.0f, 1);
        little.x = (1 - t)*s.x + t*global::mouse_x - 8;
        little.y = (1 - t)*s.y + t*global::mouse_y - 8;
        texture.draw(nullptr, &little);
      }
      glColor4f(1.0f, 1.0f, 1.0f, 1);*/
    }

     // Success
     return EXIT_SUCCESS;
  }

  int init()
  {
     ASSERT(texture.load("assets/medspeedster.png") == EXIT_SUCCESS, "Opening spaceship texture");

     // Success
     return EXIT_SUCCESS;
  }

  int spawn(float x, float y)
  {
    spaceship_t *s = new spaceship_t;

    s->x = x;
    s->y = y;
    s->dx = 0.0f;
    s->dy = 1.0f;
    s->update_on_even = (event_frame * )

    instances.push_back(s);

    // Success
    return EXIT_SUCCESS;
  }
}
