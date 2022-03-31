#include "data.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>

int init_gmt(game_map_t *map, int width, int height)
{
    map->e_height = height;
    map->e_width = width;
    map->data = (unsigned char **)malloc(height * sizeof(unsigned char *));
    if (width > MAX_WIDTH || height > MAX_HEIGHT)
    {
        wlog("Game initialization", "Width or height out of range");
        return -1;
    }
    for (int i = 0; i < height; i++)
    {
        (map->data)[i] = (unsigned char *)malloc(width * sizeof(unsigned char));
        for (int j = 0; j < width; j++)
        {
            map->data[i][j] = 0;
        }
    }
    return 0;
}

int deinit_gmt(game_map_t *t)
{
    for (int i = 0; i < t->e_height; i++)
    {
        free(t->data[i]);
    }
    free(t->data);
}

// returns 1 if everything went ok, else returns -1
int add_to_map_rect(game_map_t *game_map, unsigned char type, int tlx, int tly, int brx, int bry)
{
    int retval = 1;
    // checks
    if (game_map->e_height < bry || game_map->e_width < brx)
    {
        retval = -1;
    }
    else
    {
        for (int i = tly; i < bry; ++i)
        {
            for (int j = tlx; j < brx; ++j)
            {
                game_map->data[i][j] = type;
            }
        }
    }
    return retval;
}

int add_to_map_point(game_map_t *game_map, unsigned char type, int x, int y)
{
    int retval = 1;
    if (game_map->e_height < y || game_map->e_width < x)
    {
        retval = -1;
    }
    else
    {
        game_map->data[y][x] = type;
    }
    return retval;
}

// return 0 if not possible, return 1 if possible, return -1 if dead
int move_hero(game_map_t *game_map, point *dest)
{
    int retval = 0;
    // out of bound
    if ((dest->x >= game_map->e_width) || (dest->x < 0) || (dest->y >= game_map->e_height) || (dest->y < 0))
    {
        retval = 0;
    }
    else
    {
        unsigned char c = game_map->data[dest->y][dest->x];
        if (c == TRAP_T)
        {
            retval = -1;
        }
        else if (c != 0)
        {
            retval = 0;
        }
        else
        {
            game_map->data[game_map->hero_pos.y][game_map->hero_pos.x] = 0;            
            game_map->data[dest->y][dest->x] = HERO_P;
            game_map->hero_pos.x = dest->x;
            game_map->hero_pos.y = dest->y;
            retval = 1;
        }
    }

    return retval;
}