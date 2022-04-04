#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "parser.h"
#include "../data.h"
#include "../log.h"
#include "aux.h"

// base parser, gets only comments, might be useful... 
int parser(FILE *src)
{
    int SIGNAL_ERROR = 0;
    char c_curr;

    while (!SIGNAL_ERROR)
    {
        c_curr = (char)fgetc(src);
        switch (c_curr)
        {
        case EOF:
        {
            SIGNAL_ERROR = BUFFER_END;
            break;
        }
        case ' ':
            // printf("%c", c_curr);
            break;
        case '\n':
            // printf("%c", c_curr);
            break;
        case '\t':
            // printf("%c", c_curr);
            break;
        // CASE COMMENT
        case '(':
        {
            // printf("%c", c_curr);
            // printf("COMMENT\n");
            c_curr = (char)fgetc(src);
            // printf("%c", c_curr);
            if (c_curr != '*')
                SIGNAL_ERROR = SYNTAX_ERROR;
            else
            {
                int EOComm = 0;
                // proceed until end of the comment *)
                while (!EOComm && !SIGNAL_ERROR)
                {
                    c_curr = (char)fgetc(src);
                    // printf("%c", c_curr);
                    if (c_curr == EOF)
                        SIGNAL_ERROR = SYNTAX_ERROR;
                    else if (c_curr == '*')
                    {
                        c_curr = (char)fgetc(src);
                        // printf("%c", c_curr);
                        if (c_curr == ')')
                        {
                            EOComm = 1;
                            // printf("END OF COMMENT");
                        }
                        else if (c_curr == EOF)
                            SIGNAL_ERROR = SYNTAX_ERROR;
                    }
                }
            }
            break;
        }
        default:
        {
            SIGNAL_ERROR = SYNTAX_ERROR;
            break;
        }
        }
    }

    return SIGNAL_ERROR;
}

int parser_map(FILE *src, game_map_t *map)
{
    int SIGNAL_ERROR = 0;
    char c_curr;

    while (!SIGNAL_ERROR)
    {
        c_curr = (char)fgetc(src);
        switch (c_curr)
        {
        case EOF:
        {
            SIGNAL_ERROR = BUFFER_END;
            break;
        }
        case ' ':
            break;
        case '\n':
            break;
        case '\t':
            break;
        // CASE COMMENT
        case '(':
        {
            c_curr = (char)fgetc(src);
            if (c_curr != '*')
                SIGNAL_ERROR = SYNTAX_ERROR;
            else
            {
                int EOComm = 0;
                while (!EOComm && !SIGNAL_ERROR)
                {
                    c_curr = (char)fgetc(src);
                    if (c_curr == EOF)
                        SIGNAL_ERROR = SYNTAX_ERROR;
                    else if (c_curr == '*')
                    {
                        c_curr = (char)fgetc(src);
                        if (c_curr == ')')
                        {
                            EOComm = 1;
                        }
                        else if (c_curr == EOF)
                            SIGNAL_ERROR = SYNTAX_ERROR;
                    }
                }
            }
            break;
        }
        case '$':
        {
            // define a new object
            char *ob_name = get_next_word(src);
            if (ob_name == NULL)
            {
                SIGNAL_ERROR = SYNTAX_ERROR;
            }
            // SET MAP DIMENSION
            else if (strcmp(ob_name, "dimensions") == 0)
            {
                int width = get_next_number(src);
                int height = get_next_number(src);
                if (width != -1 && height != -1)
                    init_gmt(map, width, height);
                else
                    SIGNAL_ERROR = SYNTAX_ERROR;
            }
            else if (strcmp(ob_name, "wall") == 0)
            {
                int top_left_x = get_next_number(src);
                int top_left_y = get_next_number(src);
                int bottom_right_x = get_next_number(src);
                int bottom_right_y = get_next_number(src);
                if (top_left_x != -1 && top_left_x != -1 && top_left_x != -1 && top_left_x != -1)
                {
                    add_to_map_rect(map, WALL_T, top_left_x, top_left_y, bottom_right_x, bottom_right_y);
                }
                else
                    SIGNAL_ERROR = SYNTAX_ERROR;
            }
            else if (strcmp(ob_name, "doorv") == 0) 
            {
                int x = get_next_number(src);
                int y = get_next_number(src);
                if (x != -1 && y != -1)
                {
                    add_to_map_point(map, DOORV_T, x, y);
                }
                else
                    SIGNAL_ERROR = SYNTAX_ERROR;
            }
            else if (strcmp(ob_name, "doorh") == 0) 
            {
                int x = get_next_number(src);
                int y = get_next_number(src);
                if (x != -1 && y != -1)
                {
                    add_to_map_point(map, DOORH_T, x, y);
                }
                else
                    SIGNAL_ERROR = SYNTAX_ERROR;
            }
            else if (strcmp(ob_name, "key") == 0) 
            {
                int x = get_next_number(src);
                int y = get_next_number(src);
                if (x != -1 && y != -1)
                {
                    add_to_map_point(map, KEY_T, x, y);
                }
                else
                    SIGNAL_ERROR = SYNTAX_ERROR;
            }
            else if (strcmp(ob_name, "trap") == 0)
            {
                int x = get_next_number(src);
                int y = get_next_number(src);
                if (x != -1 && y != -1)
                {
                    add_to_map_point(map, TRAP_T, x, y);
                }
                else
                    SIGNAL_ERROR = SYNTAX_ERROR;
            }
            else if (strcmp(ob_name, "bed") == 0)
            {
                int x = get_next_number(src);
                int y = get_next_number(src);
                if (x != -1 && y != -1)
                {
                    add_to_map_point(map, BED_T, x, y);
                }
                else
                    SIGNAL_ERROR = SYNTAX_ERROR;
            }
            else if (strcmp(ob_name, "mag_chest") == 0)
            {
                int x = get_next_number(src);
                int y = get_next_number(src);
                if (x != -1 && y != -1)
                {
                    add_to_map_point(map, MAGICAL_CHEST_T, x, y);
                }
                else
                    SIGNAL_ERROR = SYNTAX_ERROR;
            }
            else if (strcmp(ob_name, "gen_chest") == 0)
            {
                int x = get_next_number(src);
                int y = get_next_number(src);
                if (x != -1 && y != -1)
                {
                    add_to_map_point(map, GENERAL_CHEST_T, x, y);
                }
                else
                    SIGNAL_ERROR = SYNTAX_ERROR;
            }
            else if (strcmp(ob_name, "ghost") == 0)
            {
                int x = get_next_number(src);
                int y = get_next_number(src);
                if (x != -1 && y != -1)
                {
                    assert(map->n_ghosts < MAX_GHOSTS);
                    add_to_map_point(map, GHOST_T, x, y);
                    map->ghosts[map->n_ghosts].x = x;
                    map->ghosts[map->n_ghosts].y = y;
                    map->n_ghosts++;
                }
                else
                    SIGNAL_ERROR = SYNTAX_ERROR;
            }
            else if (strcmp(ob_name, "zombie") == 0)
            {
                int x = get_next_number(src);
                int y = get_next_number(src);
                if (x != -1 && y != -1)
                {
                    assert(map->n_zombies < MAX_ZOMBIES);
                    add_to_map_point(map, ZOMBIE_T, x, y);
                    map->zombies[map->n_zombies].x = x;
                    map->zombies[map->n_zombies].y = y;
                    map->n_zombies++;
                }
                else
                    SIGNAL_ERROR = SYNTAX_ERROR;
            }
            else if (strcmp(ob_name, "portal") == 0)
            {
                int x = get_next_number(src);
                int y = get_next_number(src);
                char *path = get_next_word(src);
                if (path == NULL)
                {
                    SIGNAL_ERROR = SYNTAX_ERROR;
                }
                else if (x != -1 && y != -1)
                {
                    assert(map->n_portals < MAX_PORTALS);
                    add_to_map_point(map, PORTAL_T, x, y);
                    map->portals[map->n_portals].pos.x = x;
                    map->portals[map->n_portals].pos.y = y;
                    strcpy(map->portals[map->n_portals].path, path);
                    free(path);
                    map->n_portals++;
                }
                else
                    SIGNAL_ERROR = SYNTAX_ERROR;
            }
            else if (strcmp(ob_name, "start_p") == 0)
            {
                int x = get_next_number(src);
                int y = get_next_number(src);
                if (x != -1 && y != -1)
                {
                    map->starting_point.x = x;
                    map->starting_point.y = y;
                    map->hero->pos.x = map->starting_point.x; 
                    map->hero->pos.y = map->starting_point.y;
                    add_to_map_point(map, HERO_ID_T, x, y);
                }
                else
                    SIGNAL_ERROR = SYNTAX_ERROR;
            }
            else if (strcmp(ob_name, "end_p") == 0)
            {
                int x = get_next_number(src);
                int y = get_next_number(src);
                if (x != -1 && y != -1)
                {
                    map->ending_point.x = x;
                    map->ending_point.y = y;
                    add_to_map_point(map, ENDING_P, x, y);
                }
                else
                    SIGNAL_ERROR = SYNTAX_ERROR;
            }
            else if (strcmp(ob_name, "next_map") == 0)
            {
                char *path = get_next_word(src);
                if (path == NULL)
                {
                    SIGNAL_ERROR = SYNTAX_ERROR;
                }
                else
                {
                    strcpy(map->next_map, path);
                    free(path);
                }
            }
            else
            {
                SIGNAL_ERROR = SYNTAX_ERROR;
            }
            free(ob_name);

            break;
        }
        default:
        {
            SIGNAL_ERROR = SYNTAX_ERROR;
            break;
        }
        }
    }

    return SIGNAL_ERROR;
}

int parser_config_file(FILE *src, config_t *config)
{
    int SIGNAL_ERROR = 0;
    char c_curr;

    while (!SIGNAL_ERROR)
    {
        c_curr = (char)fgetc(src);
        switch (c_curr)
        {
        case EOF:
        {
            SIGNAL_ERROR = BUFFER_END;
            break;
        }
        case ' ':
            break;
        case '\n':
            break;
        case '\t':
            break;
        // CASE COMMENT
        case '(':
        {
            c_curr = (char)fgetc(src);
            if (c_curr != '*')
                SIGNAL_ERROR = SYNTAX_ERROR;
            else
            {
                int EOComm = 0;
                // proceed until end of the comment *)
                while (!EOComm && !SIGNAL_ERROR)
                {
                    c_curr = (char)fgetc(src);
                    if (c_curr == EOF)
                        SIGNAL_ERROR = SYNTAX_ERROR;
                    else if (c_curr == '*')
                    {
                        c_curr = (char)fgetc(src);
                        if (c_curr == ')')
                        {
                            EOComm = 1;
                        }
                        else if (c_curr == EOF)
                            SIGNAL_ERROR = SYNTAX_ERROR;
                    }
                }
            }
            break;
        }
        case '$':
        {
            // define a new object
            char *ob_name = get_next_word(src);
            if (ob_name == NULL)
            {
                SIGNAL_ERROR = SYNTAX_ERROR;
            }
            else if (strcmp(ob_name, "path") == 0)
            {
                char *path = get_next_word(src);
                if (path == NULL)
                {
                    SIGNAL_ERROR = SYNTAX_ERROR;
                }
                else
                {
                    strcpy(config->path_initial_map, path);
                    free(path);
                }
                free(ob_name);
            }
            else
            {
                SIGNAL_ERROR = SYNTAX_ERROR;
                free(ob_name);
            }
            break;
        }
        default:
        {
            SIGNAL_ERROR = SYNTAX_ERROR;
            break;
        }
        }
    }

    return SIGNAL_ERROR;
}
