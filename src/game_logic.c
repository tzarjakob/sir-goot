#include <ncurses.h>
#include <string.h>
#include "log.h"
#include "data.h"
#include "parsing/parser.h"
#include "game_logic.h"
#include "render/screen.h"
#include "parsing/loading.h"

int handle_movements(WINDOW *map_win, game_map_t *game_map, int dest_x, int dest_y)
{
    point dest;
    dest.x = dest_x;
    dest.y = dest_y;
    point source;
    source.x = game_map->hero_pos.x;
    source.y = game_map->hero_pos.y;
    int movement_res = move_hero(game_map, &dest);
    if (movement_res == MOV_POSSIBLE)
    {
        render_pixel(map_win, HERO_ID_T, dest.x, dest.y);
        render_pixel(map_win, 0, source.x, source.y);
        wrefresh(map_win);
    }
    return movement_res;
}

int game_loop(const char *path, int WIDTH, int HEIGHT)
{
    int retval = 1;
    clear();
    config_t config;
    int p_conf_res = load_game_config_from_file(path, &config);
    if (p_conf_res == BUFFER_END)
    {
        WINDOW *map_win;
        game_map_t game_map;
        if (load_game_map(map_win, &game_map, config.path_initial_map) == 1)
        {
            map_win = newwin(game_map.e_height + 2,
                             game_map.e_width + 2,
                             ((HEIGHT / 2) - (game_map.e_height / 2)),
                             ((WIDTH / 2) - (game_map.e_width / 2)));
            refresh();
            render_map(map_win, &game_map);
            char c;
            do
            {
                noecho();
                point dest;
                point start;
                int movement_res = MOV_NOT_POSSIBLE;
                c = getch();
                switch (c)
                {
                case 'q':
                {
                    delwin(map_win);
                    deinit_gmt(&game_map);
                    break;
                }
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
                case MOV_DEAD:
                {
                    c = 'q';

                    delwin(map_win);
                    deinit_gmt(&game_map);
                    break;
                }
                case MOV_WIN:
                {
                    if (strcmp(game_map.next_map, "___") == 0)
                    {
                        c = 'q';
                        delwin(map_win);
                        deinit_gmt(&game_map);
                        clear();
                        mvwprintw_center(stdscr, 6, WIDTH, "HAI VINTO");
                    }
                    else
                    {
                        clear();
                        char new_path_map[BUFFERSIZE];
                        strcpy(new_path_map, game_map.next_map);
                        delwin(map_win);
                        deinit_gmt(&game_map);
                        if (load_game_map(map_win, &game_map, new_path_map) != 1)
                        {
                            c = 'q';
                        }
                        else
                        {
                            map_win = newwin(game_map.e_height + 2,
                                             game_map.e_width + 2,
                                             ((HEIGHT / 2) - (game_map.e_height / 2)),
                                             ((WIDTH / 2) - (game_map.e_width / 2)));
                            refresh();
                            box(map_win, 0, 0);
                            wrefresh(map_win);
                            render_map(map_win, &game_map);
                        }
                    }
                    break;
                }
                }
            } while (c != 'q');
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
/* void enter_game_path(char *path)
{
    strcpy(path, "data/jakob/config.gigi");
} */

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
            // TODOO add the option to choose which map to play
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
