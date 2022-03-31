#include <ncurses.h>
#include <string.h>
#include "log.h"
#include "data.h"
#include "parser.h"
#include "render.h"

void render_pixel(WINDOW *win, unsigned char c, int width, int height)
{
    // switch type
    width++; height++;
    switch (c)
    {
    case WALL_T:
    {
        mvwprintw(win, height, width, "#");
        break;
    }
    case BOUNCING_T:
    {
        mvwprintw(win, height, width, "=");
        break;
    }
    case TRAP_T:
    {
        mvwprintw(win, height, width, "&");
        break;
    }
    default:
        break;
    }
}

void render_map(WINDOW *win, game_map_t *map)
{
    for (int i = 0; i < map->e_height; ++i)
    {
        for (int j = 0; j < map->e_width; ++j)
        {
            render_pixel(win, map->data[i][j], j, i);
        }
    }
    wrefresh(win);
}



int load_config(const char *path, config_t *game_config)
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

int load_game_map_s(FILE *game_file, game_map_t *game_map)
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

int game_loop(const char *path, int WIDTH, int HEIGHT)
{
    clear();
    config_t config;
    int p_conf_res = load_config(path, &config);
    if (p_conf_res == BUFFER_END)
    {
        game_map_t game_map;
        game_map.e_height = -1; game_map.e_width = -1;
        FILE *game_file = fopen(config.path_initial_map, "r");
        if (game_file == NULL)
        {
            wlog("File opening", "Failed to open game_file from path of config type");
            return -1;
        }
        int pars_map_res = load_game_map_s(game_file, &game_map);
        fclose(game_file);

        if ((game_map.e_width == -1) || (game_map.e_height == -1))
        {
            wlog("Map parsing","Width or height not specified");
            return -1;
        }

        if (pars_map_res == BUFFER_END)
        {
            // 3. adjust the size of this window
            WINDOW *map_win = newwin(game_map.e_height + 2,
                                     game_map.e_width + 2,
                                     ((HEIGHT / 2) - (game_map.e_height / 2)),
                                     ((WIDTH / 2) - (game_map.e_width / 2)));
            refresh();
            box(map_win, 0, 0);
            wrefresh(map_win);

            char c;
            do
            {
                render_map(map_win, &game_map);
                noecho();
                c = getch();
                switch (c)
                {
                case 'q':

                    break;

                default:
                    break;
                }
            } while (c != 'q');

            delwin(map_win);
            deinit_gmt(&game_map);
        }
    }
    return 1;
}

// receives as input the specific game to play and tells the function game_loop what is the first map to load
void load_map_option(char *path)
{
    strcpy(path, "/home/tzarjakob/TzarJakob/CS_PROGETTI/NCURSES_GAME/render/data/jakob/config.gigi");
}

void render_main_screen(const int WIDTH, const int HEIGHT)
{
    int line = 4;
    mvwprintw_center(stdscr, line++, WIDTH, "This is the main screen");
    mvwprintw_center(stdscr, line++, WIDTH, "Press l to load a game");
    mvwprintw_center(stdscr, line++, WIDTH, "Press q to quit");
}

void main_screen(const int WIDTH, const int HEIGHT)
{
    char c;
    do
    {
        noecho();
        clear();
        render_main_screen(WIDTH, HEIGHT);
        c = getch();
        switch (c)
        {
        case 'i':
            break;
        case 'a':
            break;
        case 'l':
        {
            wlog("main screen", "pressed l");
            char path[BUFFER_LEN] = "/home/tzarjakob/TzarJakob/CS_PROGETTI/NCURSES_GAME/render/data/jakob/config.gigi";
            // load_map_option(path);
            int res = game_loop(path, WIDTH, HEIGHT);
            char res_str[BUFFER_LEN];
            sprintf(res_str, "%d", res);
            wlog("Game loop retval", res_str);
            break;
        }
        case 's':
            break;
        case 'q':
            // quitting
            break;
        default:
            // log()
            break;
        }
    } while (c != 'q');
}