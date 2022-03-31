#ifndef LOG_H
#define LOG_H

#include <ncurses.h>

int wlog(const char *topic, const char *text);
int wlog_int(const char *topic, int x);

#endif 
