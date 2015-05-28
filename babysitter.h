#pragma once

#include <functional>

namespace babbysitter
{
  int update(float dt);

  int sit(std::function<float(float, float)> baby_step);
}
