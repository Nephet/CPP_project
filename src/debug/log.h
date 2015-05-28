#pragma once

#define LOG_INFO 0
#define LOG_WARN 1
#define LOG_ERROR 2

void log(unsigned int level, const char* format, ...);

void log(const char* format, ...);
