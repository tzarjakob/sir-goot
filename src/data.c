#include "data.h"
#include <stdio.h>
#include <stdlib.h>


int wlog(const char* topic, const char *text)
{
    FILE* log_file = fopen("/home/tzarjakob/TzarJakob/CS_PROGETTI/NCURSES_GAME/render/log/main.log", "w");
    if (log_file < 0)
    {
        return -1;
    }
    fprintf(log_file, "LOG on %s: \t %s \n", topic, text);
    fclose(log_file);
    return 0;
}

int init_gmt(game_map_t* map, int width, int height){
    map->e_height = height; map->e_width = width;
    map->data = (unsigned char**) malloc (height * sizeof(unsigned char*));
    if (width > MAX_WIDTH || height > MAX_HEIGHT) {
        wlog("Game initialization", "Width or height out of range");
        return -1;
    }
    for(int i=0; i<height; i++) {
        (map->data)[i] = (unsigned char*) malloc (width * sizeof(unsigned char));
    }
    return 0;
}

int deinit_gmt(game_map_t* t){
    for(int i=0; i<t->e_height; i++) {
        free(t->data[i]);
    }
    free(t->data);
}

// returns 1 if everything went ok, else returns -1
int add_to_map(game_map_t* game_map, int tlx, int tly, int brx, int bry)
{
    int retval = 1;
    // checks
    if (game_map->e_height < bry || game_map->e_width < brx)
    {
        retval = -1;
    } else 
    {
        
    }
    return retval;
}


