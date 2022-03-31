#ifndef PARSER_H
#define PARSER_H
#include "data.h"

int parser_map(FILE *src, game_map_t* map);
int parser_config_file(FILE *src, config_t* con);

#endif
