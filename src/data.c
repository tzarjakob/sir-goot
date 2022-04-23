#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "data.h"
#include "log.h"
#include "render/screen.h"

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
            map->data[i][j] = EMPTY_SPACE_T;
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

void move_possible()
{
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

// 0 -> wall, 1->movement possible, -2 if wins, return -1 if dead
int move_hero(game_map_t *game_map, point_t *dest)
{
    int retval = MOV_NOT_POSSIBLE;
    // out of bound
    if ((dest->x >= game_map->e_width) || (dest->x < 0) || (dest->y >= game_map->e_height) || (dest->y < 0))
    {
        retval = MOV_NOT_POSSIBLE;
    }
    else
    {
        unsigned char c = game_map->data[dest->y][dest->x];
        switch (c)
        {
        case TRAP_T:
        {
            retval = MOV_DEAD;
            break;
        }
        case ENDING_P:
        {
            retval = MOV_WIN;
            break;
        }
        case KEY_T:
        {
            game_map->data[dest->y][dest->x] = EMPTY_SPACE_T;
            game_map->hero->pos.x = dest->x;
            game_map->hero->pos.y = dest->y;
            retval = MOV_POSSIBLE;
            (game_map->hero->keys)++;
            break;
        }
        case DOORH_T:
        {
            if (game_map->hero->keys > 0)
            {
                if (confirmation_dialog("Open door", "Do you want to open the door?"))
                {
                    // effectively removes the opened door
                    game_map->data[dest->y][dest->x] = EMPTY_SPACE_T;
                    game_map->hero->pos.x = dest->x;
                    game_map->hero->pos.y = dest->y;
                    retval = MOV_POSSIBLE;
                    (game_map->hero->keys)--;
                }
            }
            else
            {
                message_dialog("Error", "You have no keys to open the door :(");
                retval = MOV_NOT_POSSIBLE;
            }
            break;
        }
        case DOORV_T:
        {
            if (game_map->hero->keys > 0)
            {
                if (confirmation_dialog("Open door", "Do you want to open the door?"))
                {
                    // effectively removes the opened door
                    game_map->data[dest->y][dest->x] = EMPTY_SPACE_T;
                    game_map->hero->pos.x = dest->x;
                    game_map->hero->pos.y = dest->y;
                    retval = MOV_POSSIBLE;
                    (game_map->hero->keys)--;
                }
            }
            else
            {
                // dialog
                message_dialog("Error", "You have no keys to open the door :(");
                retval = MOV_NOT_POSSIBLE;
            }
            break;
        }
        case GENERAL_CHEST_T:
        {
            // game_map->hero->pos.x = dest->x;
            // game_map->hero->pos.y = dest->y;
            retval = MOV_CHEST;
            break;
        }
        case MAGICAL_CHEST_T:
        {
            // open it only if you are a wizard, else display a warning
            if (game_map->hero->wizard)
            {
                // you can open the chest
                // game_map->hero->pos.x = dest->x;
                // game_map->hero->pos.y = dest->y;
                retval = MOV_CHEST;
            }
            else
            {
                // display an error message!
                retval = MOV_NOT_POSSIBLE;
            }
            break;
        }
        case PORTAL_T:
        {
            retval = MOV_PORTAL;
            break;
        }
        case EMPTY_SPACE_T:
        {
            game_map->hero->pos.x = dest->x;
            game_map->hero->pos.y = dest->y;
            retval = MOV_POSSIBLE;
            break;
        }
        default:
            retval = MOV_NOT_POSSIBLE;
            break;
        }
    }

    return retval;
}