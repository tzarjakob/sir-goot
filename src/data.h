#ifndef DATA_H
#define DATA_H

#define BUFFER_LEN  50
#define MAX_WIDTH   200
#define MAX_HEIGHT  50

#define WALL_T      1
#define BOUNCING_T  2
#define TRAP_T      3

#include <stdlib.h>
#include <stdio.h>

int wlog(const char *text);

typedef struct {
    unsigned char** data;
    int e_width; int e_height;
    // also it is convenient to have all the elements separately...
} game_map_t;

// rows = height

int init_gmt(game_map_t* map, int width, int height);
int deinit_gmt(game_map_t*, int width, int height);




#endif 
