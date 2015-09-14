
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "type.h"
#include "log.h"

static u32 debug_level;

// init debug

void init_debug()
{

	debug_level = 0;
}

// set debug level

void set_debug_level(u32 val)
{

	debug_level = val;
}

// only print error message

void log_print_error(const char *fmt, ...)
{

	va_list args;
	char buffer[1024];

	if (debug_level >= LOG_ERROR_LEVEL) {

		va_start(args, fmt);
		vsprintf(buffer, fmt, args);
		va_end(args);

		printf("%s", buffer);
	}

}

// print only warn message

void log_print_warn(const char *fmt, ...)
{

	va_list args;
	char buffer[1024];

	if (debug_level >= LOG_WARN_LEVEL) {

		va_start(args, fmt);
		vsprintf(buffer, fmt, args);
		va_end(args);

		printf("%s", buffer);
	}

}

// print normal log message

void log_print_msg(const char *fmt, ...)
{

	va_list args;
	char buffer[1024];

	if (debug_level >= LOG_MSG_LEVEL) {

		va_start(args, fmt);
		vsprintf(buffer, fmt, args);
		va_end(args);

		printf("%s", buffer);
	}
}
