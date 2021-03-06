#ifndef DATA_H
#define DATA_H


#define MIN_WIDTH 60
#define MIN_HEIGHT 15
#define MAX_WIDTH 200
#define MAX_HEIGHT 50
// timeout of each frame
#define GAME_SPEED 1000
#define MAXIMUM_GAMES 10

#define UPDATE_STAT_WIN 1000

#define MAX_TIME_DIALOG_DURATION 5000


/* COLORS */
#define MAIN_THEME 1
#define SECONDARY_THEME 2
#define MAIN_SCREEN_THEME 3
#define BLACK_YELLOW_THEME 4
#define WHITE_RED_THEME 5

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
#define GENERAL_CHEST_T (unsigned char)12
#define MAGICAL_CHEST_T (unsigned char)13
#define BED_T (unsigned char)14
#define PORTAL_T (unsigned char)15

#define INITIAL_KEYS 4
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
#define MOV_PORTAL 4
#define MOV_CHEST 5

#define BUFFERSIZE 256

#define STAT_WIN_WIDTH 50
#define INV_WIN_WIDTH 30
#define INV_WIN_HEIGHT 20

#define GHOST_DAMAGE 2
#define ZOMBIE_DAMAGE 1

#define MAX_PORTALS 10
#define MAX_ZOMBIES 50
#define MAX_GHOSTS 50

// #define CONFIG_INITIAL_PATH "data/johan/config.gigi"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct
{
    int x;
    int y;
} point_t;

typedef struct 
{
    point_t pos;
    char path[BUFFERSIZE];
} portal_t;

typedef struct 
{
    // weapons
    // 
} inventory_t;

typedef struct
{
    char path_initial_map[BUFFERSIZE];
} config_t;

typedef struct
{
    point_t pos;
    int lives;
    int experience;
    int keys;
    int money;
    bool wizard;
    inventory_t inventory;
} hero_t;

typedef struct
{
    unsigned char **data;
    int e_width;
    int e_height;
    point_t starting_point;
    point_t ending_point;
    portal_t portals[MAX_PORTALS];
    int n_portals;
    hero_t *hero;
    point_t ghosts[MAX_GHOSTS];
    int n_ghosts;
    point_t zombies[MAX_ZOMBIES];
    int n_zombies;
    char next_map[BUFFERSIZE];
} game_map_t;

int init_gmt(game_map_t *map, int width, int height);
int deinit_gmt(game_map_t *);
int add_to_map_rect(game_map_t *game_map, unsigned char type, int tlx, int tly, int brx, int bry);
int add_to_map_point(game_map_t *game_map, unsigned char type, int x, int y);
int move_hero(game_map_t *game_map, point_t *dest);
char *destination_right_portal(game_map_t *game_map, point_t *dest);

#endif
