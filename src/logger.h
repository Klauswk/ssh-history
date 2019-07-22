#ifndef LOGGER_H
#define LOGGER_H
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#ifndef DEBUG_FLAG
#define DEBUG_FLAG 1

#endif

void log_format(const char *tag, const char *message, va_list args);

void log_error(const char *message, ...);

void log_info(const char *message, ...);

void log_debug(const char *message, ...);


#endif