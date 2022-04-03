#ifndef DATA_H
#define DATA_H

#define MAX_WIDTH 200
#define MAX_HEIGHT 50

// data types in the map
#define EMPTY_SPACE_T (unsigned char)0
#define WALL_T (unsigned char)1
#define BOUNCING_T (unsigned char)2
#define TRAP_T (unsigned char)3
#define STARTING_P (unsigned char)4
#define ENDING_P (unsigned char)5
#define HERO_ID_T (unsigned char)6
#define DOORV_T (unsigned char)7
#define DOORH_T (unsigned char)8
#define KEY_T (unsigned char)9
#define GHOST_T (unsigned char)10
#define ZOMBIE_T (unsigned char)11

#define INITIAL_KEYS 1
#define INITIAL_MONEY 0
#define INITIAL_LIVES 3
#define INITIAL_EXP 0

// return values of parsing functions
#define BUFFER_END 1
#define LEXICAL_ERROR 2
#define SYNTAX_ERROR 3
#define SEMANTICAL_ERROR 4

#define NUMBER_NOT_FOUND -1

#define MOV_POSSIBLE 0
#define MOV_NOT_POSSIBLE 1
#define MOV_DEAD 2
#define MOV_WIN 3

#define BUFFERSIZE 100

#define STAT_WIN_WIDTH 30

#define GHOST_DAMAGE 2
#define ZOMBIE_DAMAGE 1

#define MAX_ZOMBIES 50
#define MAX_GHOSTS 50

#define CONFIG_INITIAL_PATH "data/jakob/config.gigi"

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
    point pos;
    int lives;
    int experience;
    int keys;
    int money;
} hero_t;

typedef struct
{
    unsigned char **data;
    int e_width;
    int e_height;
    point starting_point;
    point ending_point;
    hero_t *hero;
    point ghosts[MAX_GHOSTS];
    int n_ghosts;
    point zombies[MAX_ZOMBIES];
    int n_zombies;
    char next_map[BUFFERSIZE];
} game_map_t;

int init_gmt(game_map_t *map, int width, int height);
int deinit_gmt(game_map_t *);
int add_to_map_rect(game_map_t *game_map, unsigned char type, int tlx, int tly, int brx, int bry);
int add_to_map_point(game_map_t *game_map, unsigned char type, int x, int y);
int move_hero(game_map_t *game_map, point *dest);

#endif
