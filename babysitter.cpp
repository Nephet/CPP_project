#include "babysitter.h"

#include <list>

using namespace std;

namespace babysitter
{
  namespace  {
    struct baby_t
    {
      function<float(float)> step;
      float progress;
    };

    static list<baby_t*> babies;

  }

  int update(float dt)
  {
    // Update all the babies
    for(auto i = babies.begin(); i != babies.end(); i++)
    {
      baby_t &b = (**i);
      b.progress = b.step(b.progress);
    }

    // Nothing to report
    return 0;
  }

  int sit(std::function<float(float)> baby_step)
  {
    // All good
    return 0;
  }
}
