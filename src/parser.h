#ifndef PARSER_H
#define PARSER_H


#include "data.h"

int get_next_number(FILE *src);
char* get_next_word(FILE* src);
char** get_next_N_word(FILE *src, const int N);
int get_attributes(FILE *src, int N);
int parser(FILE *src);
int parser_map(FILE *src, game_map_t* map);
int parser_config_file(FILE *src, config_t* con);


#endif
