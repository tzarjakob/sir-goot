#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
            else if (strcmp(ob_name, "start_p") == 0)
            {
                int x = get_next_number(src);
                int y = get_next_number(src);
                if (x != -1 && y != -1)
                {
                    map->starting_point.x = x;
                    map->starting_point.y = y;
                    map->hero_pos.x = map->starting_point.x;
                    map->hero_pos.y = map->starting_point.y;
                    add_to_map_point(map, HERO_P, x, y);
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
                wlog("Config parsing", "PATH IDENTIFIED!!!");
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