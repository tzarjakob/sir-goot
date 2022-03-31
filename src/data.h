#ifndef DATA_H
#define DATA_H

#define BUFFER_LEN  100
#define MAX_WIDTH   200
#define MAX_HEIGHT  50

#define WALL_T      1
#define BOUNCING_T  2
#define TRAP_T      3
#define STARTING_P  4
#define ENDING_P    5

#define LEXICAL_ERROR     1
#define SYNTAX_ERROR      2
#define SEMANTICAL_ERROR  3
#define BUFFER_END        4

#define NOT_VALID_CHAR   -2
#define NUMBER_NOT_FOUND  -1

#define BUFFERSIZE        100

#define MODE_PARSER       1

#include <stdlib.h>
#include <stdio.h>

int wlog(const char* topic, const char *text);

typedef struct {
    int x; int y;
} point;

typedef struct {
    char path_initial_map[BUFFER_LEN];

} config_t;

typedef struct {
    unsigned char** data;
    int e_width; int e_height;
    point starting_point; 
    point ending_point;
    char next_map[BUFFERSIZE];
} game_map_t;

// rows = height

int init_gmt(game_map_t* map, int width, int height);
int deinit_gmt(game_map_t*);
int add_to_map_rect(game_map_t* game_map, unsigned char type, int tlx, int tly, int brx, int bry);
int add_to_map_point(game_map_t* game_map, unsigned char type, int x, int y);



#endif 
