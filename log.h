
#ifndef _LOG_H
#define _LOG_H

// add log level macro definition

#define LOG_MSG_LEVEL   0x1
#define LOG_WARN_LEVEL  0x2
#define LOG_ERROR_LEVEL 0x3

// defintion function prototype

void init_debug();
void set_debug_level();

void log_print_error(const char* fmt, ...) ;
void log_print_warn(const char* fmt, ...) ;
void log_print_msg(const char* fmt, ...) ;

#endif

