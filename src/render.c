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
        mvwprintw(win, height, width, "?");
        break;
    }
    case ENDING_P:
    {
        mvwprintw(win, height, width, "!");
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

// return 1 if everything went ok, -1 otherwise
int load_game_map(WINDOW *map_win, game_map_t *game_map, const char *path, const int WIDTH, const int HEIGHT)
{
    int retval = 1;
    game_map->e_height = -1;
    game_map->e_width = -1;
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
        else if (pars_map_res != BUFFER_END)
        {
            map_win = newwin(game_map->e_height + 2,
                             game_map->e_width + 2,
                             ((HEIGHT / 2) - (game_map->e_height / 2)),
                             ((WIDTH / 2) - (game_map->e_width / 2)));
            refresh();
            box(map_win, 0, 0);
            wrefresh(map_win);

            render_map(map_win, game_map);
        }
        else
        {
            retval = -1;
        }
    }
    return retval;
}

int game_loop(const char *path, int WIDTH, int HEIGHT)
{
    int retval = 0;
    clear();
    config_t config;
    int p_conf_res = load_game_config_from_file(path, &config);
    if (p_conf_res == BUFFER_END)
    {
        WINDOW *map_win;
        game_map_t game_map;
        if (load_game_map(map_win, &game_map, config.path_initial_map, WIDTH, HEIGHT) == 1)
        {
            char c;
            // nodelay(map_win, TRUE);
            do
            {
                noecho();
                // timeout(1000);
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
                // it would be significantly better if we could verify the current position of the hero
                switch (movement_res)
                {
                case -1:
                    c = 'q';
                    break;
                case -2:
                {
                    if (strcmp(game_map.next_map, "_"))
                    {
                        mvwprintw_center(map_win, 6, WIDTH, "HAI VINTO");
                        wrefresh(map_win);
                    }
                    else
                    {
                        char new_path_map[BUFFERSIZE];
                        strcpy(new_path_map, game_map.next_map);
                        delwin(map_win);
                        deinit_gmt(&game_map);
                        load_game_map(map_win, &game_map, new_path_map, WIDTH, HEIGHT);
                    }
                    break;
                }
                }
            } while (c != 'q');

            delwin(map_win);
            deinit_gmt(&game_map);
        }
        else 
        {
            retval = -1;
        }
    }
    else
    {
        retval = -1;
    }
    return retval;
}

// receives as input the specific game to play and tells the function game_loop what is the first map to load
void enter_game_path(char *path)
{
    strcpy(path, "data/jakob/config.gigi");
}

void render_main_screen(const int WIDTH, const int HEIGHT)
{
    int line = 4;
    box(stdscr, 0, 0);
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
