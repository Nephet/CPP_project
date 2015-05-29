#include "spaceship.h"

#include "debug/assert.h"
#include "math/Rect.hpp"
#include "graphics/Texture.hpp"

#include <list>

using namespace std;

static Texture texture;

static list<spaceship_t*> instances;


static float rot_c = cos(PI*0.01);
static float rot_s = sin(PI*0.01);
 // static fRect sprite(0, 0, 128, 128);
/*
static float spaceship_x =0;
static float spaceship_y =0;
static float spaceship_dx = 0.0f;
static float spaceship_dy = -1.0f;
*/
namespace spaceship
{

    int update(float dt)
    {

        return 0;
    }
    int draw()
    {
        for(auto i = instances.begin(); i != instances.end(); i++)
        {

            spaceship_t &s = (*i);

            sprite.x =spaceship_x - sprite.w*0.5f;
            sprite.y =spaceship_y - sprite.h*0.5f;
            // Draw the sprite
            texture.draw(nullptr, &sprite);

            float x = sprite.x + sprite.w*0.5f + spaceship_dx*128;
            float y = sprite.y + sprite.h*0.5f + spaceship_dy*128;
            fRect little(0, 0, 16, 16);

            for(float t= 0.0f; t <1.0f; t+= 0.1f)
            {

                glColor4f(1.0f,0.0f,0.0f,1.0f);
                little.x = (1-t)* spaceship_x + t*x-8;
                little.y = (1-t)* spaceship_y + t*y-8;

                texture.draw(nullptr, &little);
                glColor4f(0.0f,0.0f,1.0f,1.0f);
                little.x = (1-t)* spaceship_x + t*global::mouse_x-8;
                little.y = (1-t)* spaceship_y + t*global::mouse_y-8;

                texture.draw(nullptr, &little);
            }
            glColor4f(1.0f,1.0f,1.0f,1.0f);*/

        }
        return 0;
    }

    int init()
    {

        ASSERT(texture.load("assets/medspeedster.png") == EXIT_SUCCESS, "load spaceship");

        return 0;
    }

    int spawn(float x, float y)
    {

        //spaceship_x = global::viewport.x* 0.5f;
        //spaceship_y = global::viewport.y* 0.5f;
        return 0;
    }
}



 /* //sprite.x = (global::viewport.x - sprite.w) * 0.5f;
  //sprite.y = (global::viewport.y - sprite.h) * 0.5f;

  // Turn the ship
  float x = sprite.x - sprite.w*0.5f;
  float y = sprite.y - sprite.h*0.5f;
  float vx = global::mouse_x - spaceship_x;
  float vy = global::mouse_y - spaceship_y;
  float norm_v = sqrt(vx*vx + vy*vy);
  float nvx = vx/norm_v;
  float nvy = vy/norm_v;

  // Do we need to turn
  float dot = spaceship_dx*nvx + spaceship_dy*nvy;

  if(dot < 0.99f)
  {
    // Which direction shall we turn in?
    float det = spaceship_dx*vy - spaceship_dy*vx;
    float ss = det < 0 ? -rot_s : rot_s;

    spaceship_dx = spaceship_dx*rot_c - spaceship_dy*ss;
    spaceship_dy = spaceship_dx*ss + spaceship_dy*rot_c;
    float norm = sqrt(spaceship_dx*spaceship_dx + spaceship_dy*spaceship_dy);
    spaceship_dx /= norm;
    spaceship_dy /= norm;
  }

    if(norm_v>32)
    {
        spaceship_x += 256*spaceship_dx*dt;
        spaceship_y += 256*spaceship_dy*dt;
    }



*/
 /* //
