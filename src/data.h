#ifndef DATA_H
#define DATA_H

#define MAX_WIDTH 200
#define MAX_HEIGHT 50

// data types in the map
#define WALL_T (unsigned char)1
#define BOUNCING_T (unsigned char)2
#define TRAP_T (unsigned char)3
#define STARTING_P (unsigned char)4
#define ENDING_P (unsigned char)5
#define HERO_P (unsigned char)6

// return values of parsing functions
#define BUFFER_END 1
#define LEXICAL_ERROR 2
#define SYNTAX_ERROR 3
#define SEMANTICAL_ERROR 4

#define NUMBER_NOT_FOUND -1

#define BUFFERSIZE 100

#include <stdlib.h>
#include <stdio.h>

typedef struct
{
    int x;
    int y;
} point;

typedef struct
{
    char path_initial_map[BUFFERSIZE];

} config_t;

typedef struct
{
    unsigned char **data;
    int e_width;
    int e_height;
    point starting_point;
    point ending_point;
    char next_map[BUFFERSIZE];
    point hero_pos;
} game_map_t;

// rows = height

int init_gmt(game_map_t *map, int width, int height);
int deinit_gmt(game_map_t *);
int add_to_map_rect(game_map_t *game_map, unsigned char type, int tlx, int tly, int brx, int bry);
int add_to_map_point(game_map_t *game_map, unsigned char type, int x, int y);
int move_hero(game_map_t *game_map, point* dest);

#endif
