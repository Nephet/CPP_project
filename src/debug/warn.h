#pragma once

#include "log.h"     // for LOG

#define WARN(what, why)                             \
  log(LOG_WARN, "%s : %s", what, why);              \

#define WARN_IF(problem, what, why)                 \
{                                                   \
  if(problem)                                       \
  {                                                 \
    WARN(what, why)                                 \
  }                                                 \
}                                                   \

#define WARN_RTN(what, why, rtn)                    \
{                                                   \
  WARN(what, why)                                   \
  return(rtn);                                      \
}
