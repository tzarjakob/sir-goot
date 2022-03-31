#ifndef LOG_H
#define LOG_H

#include <ncurses.h>

void mvwprintw_center(WINDOW *win, int line, int box_width, const char *text);

#endif 
