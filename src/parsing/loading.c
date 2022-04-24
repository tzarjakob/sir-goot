#include <ncurses.h>
#include <string.h>
#include "../log.h"
#include "../data.h"
#include "parser.h"

int load_game_config_from_file(const char *path, config_t *game_config)
{
    FILE *config = fopen(path, "r");
    if (config == NULL)
    {
        return -1;
    }
    // int pars_conf_res = 2;
    int pars_conf_res = parser_config_file(config, game_config);
    switch (pars_conf_res)
    {
    case LEXICAL_ERROR:
        wlog("Config file", "LEXICAL_ERROR");
        break;
    case SYNTAX_ERROR:
        wlog("Config file", "SYNTAX_ERROR");
        break;
    case SEMANTICAL_ERROR:
        wlog("Config file", "SEMANTICAL_ERROR");
        break;
    case BUFFER_END:
        // ok
        break;
    default:
        wlog("Config file", "Some errors... ");
        break;
    }
    fclose(config);
    return pars_conf_res;
}

int load_game_map_from_file(FILE *game_file, game_map_t *game_map)
{
    int pars_map_res = parser_map(game_file, game_map);
    switch (pars_map_res)
    {
    case LEXICAL_ERROR:
        wlog("Config file", "LEXICAL_ERROR");
        break;
    case SYNTAX_ERROR:
        wlog("Config file", "SYNTAX_ERROR");
        break;
    case SEMANTICAL_ERROR:
        wlog("Config file", "SEMANTICAL_ERROR");
        break;
    case BUFFER_END:
        // ok
        break;
    default:
        wlog("Config file", "Some errors... ");
        break;
    }
    return pars_map_res;
}

// return 1 if everything went ok, -1 otherwise
int load_game_map(WINDOW *map_win, game_map_t *game_map, const char *path, const int WIDTH, const int HEIGHT)
{
    int retval = 1;
    game_map->e_height = -1;
    game_map->e_width = -1;
    game_map->n_ghosts = 0;
    game_map->n_zombies = 0;
    game_map->n_portals = 0;
    FILE *game_file = fopen(path, "r");
    if (game_file == NULL)
    {
        wlog("File opening", "Failed to open game_file from path of config type");
        retval = -1;
    }
    else
    {
        int pars_map_res = load_game_map_from_file(game_file, game_map);
        fclose(game_file);

        if ((game_map->e_width == -1) || (game_map->e_height == -1))
        {
            wlog("Map parsing", "Width or height not specified");
            retval = -1;
        }
        else if (((game_map->e_height + 4) > HEIGHT) || ((game_map->e_width + 20) > WIDTH))
        {
            wlog("Map parsing", "Width or height out of limits");
            retval = -1;
        }
        else if (pars_map_res == BUFFER_END)
        {
            retval = 1;
        }
        else
        {
            wlog("Map construction error", "Parsing error");
            retval = -1;
        }
    }
    wlog_int("Return value construction map", retval);
    return retval;
}
