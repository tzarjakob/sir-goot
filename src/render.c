#include <ncurses.h>
#include <string.h>
#include "log.h"
#include "data.h"
#include "parser.h"
#include "render.h"

void mvwprintw_center(WINDOW *win, int line, int box_width, const char *text)
{
    int x = (box_width - strlen(text)) / 2;
    mvwprintw(win, line, x, text);
}

void render_pixel(WINDOW *win, unsigned char c, int width, int height)
{
    // switch type
    width++;
    height++;
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
    case HERO_P:
    {
        mvwprintw(win, height, width, "8");
        break;
    }
    default:
    {
        mvwprintw(win, height, width, " ");
        break;
    }
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

int handle_movements(WINDOW *map_win, game_map_t *game_map, int dest_x, int dest_y)
{
    point dest;
    dest.x = dest_x;
    dest.y = dest_y;
    point source;
    source.x = game_map->hero_pos.x;
    source.y = game_map->hero_pos.y;
    int movement_res = move_hero(game_map, &dest);
    if (movement_res == 1)
    {
        render_pixel(map_win, HERO_P, dest.x, dest.y);
        render_pixel(map_win, 0, source.x, source.y);
        wrefresh(map_win);
    }
    return movement_res;
}

int game_loop(const char *path, int WIDTH, int HEIGHT)
{
    clear();
    config_t config;
    int p_conf_res = load_game_config_from_file(path, &config);
    if (p_conf_res == BUFFER_END)
    {
        game_map_t game_map;
        game_map.e_height = -1;
        game_map.e_width = -1;
        FILE *game_file = fopen(config.path_initial_map, "r");
        if (game_file == NULL)
        {
            wlog("File opening", "Failed to open game_file from path of config type");
            return -1;
        }
        int pars_map_res = load_game_map_from_file(game_file, &game_map);
        fclose(game_file);

        if ((game_map.e_width == -1) || (game_map.e_height == -1))
        {
            wlog("Map parsing", "Width or height not specified");
            return -1;
        }

        if (pars_map_res == BUFFER_END)
        {
            WINDOW *map_win = newwin(game_map.e_height + 2,
                                     game_map.e_width + 2,
                                     ((HEIGHT / 2) - (game_map.e_height / 2)),
                                     ((WIDTH / 2) - (game_map.e_width / 2)));
            refresh();
            box(map_win, 0, 0);
            wrefresh(map_win);

            char c;
            render_map(map_win, &game_map);
            do
            {
                noecho();
                point dest;
                point start;
                int movement_res = 0;
                c = getch();
                switch (c)
                {
                case 'q':

                    break;
                // Movements
                case 'w':
                {
                    movement_res = handle_movements(map_win, &game_map,
                                                        game_map.hero_pos.x, game_map.hero_pos.y - 1);
                    break;
                }
                case 'd':
                {
                    movement_res = handle_movements(map_win, &game_map,
                                                        game_map.hero_pos.x + 1, game_map.hero_pos.y);
                    break;
                }
                case 's':
                {
                    movement_res = handle_movements(map_win, &game_map,
                                                        game_map.hero_pos.x, game_map.hero_pos.y + 1);
                    break;
                }
                case 'a':
                {
                    movement_res = handle_movements(map_win, &game_map,
                                                        game_map.hero_pos.x - 1, game_map.hero_pos.y);
                    break;
                }
                default:
                    break;
                }
                if (movement_res == -1)
                {
                    c = 'q';
                }
            } while (c != 'q');

            delwin(map_win);
            deinit_gmt(&game_map);
        }
    }
    return 1;
}

// receives as input the specific game to play and tells the function game_loop what is the first map to load
void enter_game_path(char *path)
{
    strcpy(path, "data/jakob/config.gigi");
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
        case 'l':
        {
            char path[BUFFERSIZE] = "data/jakob/config.gigi";
            int res = game_loop(path, WIDTH, HEIGHT);
            break;
        }
        case 'q':
            // quitting...
            break;
        default:
            break;
        }
    } while (c != 'q');
}
