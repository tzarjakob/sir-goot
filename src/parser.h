#ifndef PARSER_H
#define PARSER_H


#define LEXICAL_ERROR     1
#define SYNTAX_ERROR      2
#define SEMANTICAL_ERROR  3
#define BUFFER_END        4

#define NOT_VALID_CHAR   -2
#define NUMBER_NOT_FOUND  -1

#define BUFFERSIZE        100

#define MODE_PARSER       1

#include "data.h"

int get_next_number(FILE *src);
char* get_next_word(FILE* src);
char** get_next_N_word(FILE *src, const int N);
int get_attributes(FILE *src, int N);
int parser(FILE *src);
int parser_map(FILE *src, game_map_t* map);
int parser_config_file(FILE *src, config_t* con);


#endif
