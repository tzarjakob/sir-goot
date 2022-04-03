#ifndef SCREEN_H
#define SCREEN_H

#include <ncurses.h>
#include <string.h>
#include "../data.h"

void mvwprintw_center(WINDOW *win, int line, int box_width, const char *text);
void render_pixel(WINDOW *win, unsigned char c, int width, int height);
void render_map(WINDOW *win, game_map_t *map);
void render_main_screen(const int WIDTH, const int HEIGHT);
void render_stat_map(WINDOW *stat_win, game_map_t *game_map, int width);

#endif 
