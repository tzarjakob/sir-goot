#ifndef SCREEN_H
#define SCREEN_H

#include <ncurses.h>
#include <string.h>
#include "../data.h"
#include <dirent.h>

void mvwprintw_center(WINDOW *win, int line, int box_width, const char *text);
void render_pixel(WINDOW *win, unsigned char c, int width, int height);
void render_map(WINDOW *win, game_map_t *map);
void render_main_screen(const int WIDTH, const int HEIGHT);
void render_stat_map(WINDOW *stat_win, game_map_t *game_map, int width);
void show_inventory(WINDOW* inv_win, game_map_t* game_map);
void show_chest(WINDOW* chest_win, game_map_t* game_map);
void render_hero(WINDOW* map_win, hero_t* hero);
void clear_hero_render(WINDOW* map_win, hero_t* hero);
int choose_index(char dirs[MAXIMUM_GAMES][BUFFERSIZE], int n);
bool confirmation_dialog(const char *title, const char *question);

#endif 
