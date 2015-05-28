#include <stdarg.h>
#include <stdio.h>          // for printf


#include "log.h"

// private function
void log(unsigned int level, const char* format, va_list& arguments)
{
  // print a header depending on the level
  static const char* LOG_STR[3] = { "INFO - ", "WARN - ", "ERROR - " };
  printf("%s", LOG_STR[(level > 2) ? 2 : level]);

  // send to standard output and flush
  vprintf(format, arguments);
  puts("");
}

void log(unsigned int level, const char* format, ...)
{
#ifdef DEBUG
  // get arguments
  va_list arguments;
  va_start(arguments, format);

  // send to auxillary function
  log(level, format, arguments);

  // don't forget to clean up!
  va_end(arguments);
#endif // #ifdef DEBUG
}

void log(const char* format, ...)
{
#ifdef DEBUG
  // get arguments
  va_list arguments;
  va_start(arguments, format);

  // send to auxillary function
  log(LOG_INFO, format, arguments);

  // don't forget to clean up!
  va_end(arguments);
#endif // #ifdef DEBUG
}
