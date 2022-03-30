#include "data.h"
#include <stdio.h>
#include <stdlib.h>


int wlog(const char *text)
{
    FILE* log_file = fopen("/home/tzarjakob/TzarJakob/CS_PROGETTI/NCURSES_GAME/render/log/main.log", "w");
    if (log_file < 0)
    {
        return -1;
    }
    fprintf(log_file, "LOG on *topic*: \t %s \n", text);
    fclose(log_file);
    return 0;
}

int init_gmt(game_map_t* map, int width, int height){
    map->e_height = height; map->e_width = width;
    map->data = (unsigned char**) malloc (height * sizeof(unsigned char*));
    if (width > MAX_WIDTH || height > MAX_HEIGHT) {
        wlog("Width or height out of range");
        return -1;
    }
    for(int i=0; i<height; i++) {
        (map->data)[i] = (unsigned char*) malloc (width * sizeof(unsigned char));
    }
    return 0;
}

int deinit_gmt(game_map_t* t, int width, int height){
    for(int i=0; i<height; i++) {
        free(t->data[i]);
    }
    free(t->data);
}


