/*
 * Copyright 2015 tristan_fei <feixiaoxing@163.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "type.h"
#include "log.h"

static u32 debug_level;

// init debug

void init_debug() {

	debug_level = 0;
}

// set debug level

void set_debug_level(u32 val) {

	debug_level = val;
}

// only print error message

void log_print_error(const char *fmt, ...) {

	va_list args;
	char buffer[1024];

	if (debug_level >= LOG_ERROR_LEVEL) {

		va_start(args, fmt);
		vsprintf(buffer, fmt, args);
		va_end(args);

		printf("%s\n", buffer);
	}

}

// print only warn message

void log_print_warn(const char *fmt, ...) {

	va_list args;
	char buffer[1024];

	if (debug_level >= LOG_WARN_LEVEL) {

		va_start(args, fmt);
		vsprintf(buffer, fmt, args);
		va_end(args);

		printf("%s\n", buffer);
	}

}

// print normal log message

void log_print_msg(const char *fmt, ...) {

	va_list args;
	char buffer[1024];

	if (debug_level >= LOG_MSG_LEVEL) {

		va_start(args, fmt);
		vsprintf(buffer, fmt, args);
		va_end(args);

		printf("%s\n", buffer);
	}
}
