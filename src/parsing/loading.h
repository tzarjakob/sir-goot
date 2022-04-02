#ifndef LOADING_H
#define LOADING_H

#include <ncurses.h>
#include <string.h>
#include "../log.h"
#include "../data.h"
#include "parser.h"


int load_game_config_from_file(const char *path, config_t *game_config);
int load_game_map_from_file(FILE *game_file, game_map_t *game_map);
// return 1 if everything went ok, -1 otherwise
int load_game_map(WINDOW *map_win, game_map_t *game_map, const char *path);

#endif 
