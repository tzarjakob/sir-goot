#include <ncurses.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <unistd.h>
#include "log.h"
#include "data.h"
#include "parsing/parser.h"
#include "game_logic.h"
#include "render/screen.h"
#include "parsing/loading.h"

/*
Using this leads to segmentation fault.
I assume because newwin allocates on the stack and not on the heap
void init_map(WINDOW* map_win, WINDOW* stat_win, const int HEIGHT, const int WIDTH, game_map_t* game_map)
{
    map_win = newwin(game_map->e_height + 2,
                     game_map->e_width + 2,
                     ((HEIGHT / 2) - (game_map->e_height / 2)) - 2,
                     ((WIDTH / 2) - (game_map->e_width / 2)) - (STAT_WIN_WIDTH / 2));
    stat_win = newwin(game_map->e_height + 2,
                      STAT_WIN_WIDTH,
                      ((HEIGHT / 2) - (game_map->e_height / 2)) - 2,
                      ((WIDTH / 2) + (game_map->e_width / 2)) - (STAT_WIN_WIDTH / 2) + 2);
    refresh();
    // box(map_win, 0, 0);
    // wrefresh(map_win);
    render_map(map_win, game_map);
    render_stat_map(stat_win, game_map, STAT_WIN_WIDTH);
} */


// TODO fix this function and the return value of move_hero function
// -> hero movement -> only delete the char and add the char; else -> render everything
int handle_movements(WINDOW *map_win, WINDOW *stat_win, game_map_t *game_map, point_t *dest)
{
    int movement_res = move_hero(game_map, dest);
    if (movement_res == MOV_PORTAL)
    {
    }
    else
    {
        render_map(map_win, game_map);
        render_stat_map(stat_win, game_map, STAT_WIN_WIDTH);
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
        WINDOW *stat_win;
        game_map_t game_map;
        hero_t hero;
        hero.experience = INITIAL_EXP;
        hero.keys = INITIAL_KEYS;
        hero.lives = INITIAL_LIVES;
        hero.money = INITIAL_MONEY;
        hero.wizard = false;
        game_map.hero = &hero;
        if (load_game_map(map_win, &game_map, config.path_initial_map, WIDTH, HEIGHT) == 1)
        {
            map_win = newwin(game_map.e_height + 2,
                             game_map.e_width + 2,
                             ((HEIGHT / 2) - (game_map.e_height / 2)) - 2,
                             ((WIDTH / 2) - (game_map.e_width / 2)) - (STAT_WIN_WIDTH / 2));
            stat_win = newwin(game_map.e_height + 2,
                              STAT_WIN_WIDTH,
                              ((HEIGHT / 2) - (game_map.e_height / 2)) - 2,
                              ((WIDTH / 2) + (game_map.e_width / 2)) - (STAT_WIN_WIDTH / 2) + 2);
            refresh();
            render_map(map_win, &game_map);
            render_stat_map(stat_win, &game_map, STAT_WIN_WIDTH);
            char c;
            int tmp = 1;
            // GAME LOOP
            do
            {
                noecho();
                render_hero(map_win, &hero);

                point_t dest;
                point_t start;
                int movement_res = MOV_NOT_POSSIBLE;
                c = getch();

                // it clear the rendering of the hero
                clear_hero_render(map_win, &hero);
                switch (c)
                {
                case 'e':
                {
                    // show inventary window -> fixed dimension
                    WINDOW *inv_win = newwin(INV_WIN_HEIGHT,
                                             INV_WIN_WIDTH,
                                             (HEIGHT / 2) - (INV_WIN_HEIGHT / 2),
                                             (WIDTH / 2) - (INV_WIN_WIDTH / 2));
                    show_inventory(inv_win, &game_map);
                    getch();
                    delwin(inv_win);
                    wclear(stdscr);
                    refresh();
                    render_map(map_win, &game_map);
                    render_stat_map(stat_win, &game_map, STAT_WIN_WIDTH);
                    break;
                }
                case 'q':
                {
                    delwin(map_win);
                    deinit_gmt(&game_map);
                    break;
                }
                // Movements
                case 'w':
                {
                    dest.x = game_map.hero->pos.x;
                    dest.y = game_map.hero->pos.y - 1;
                    movement_res = handle_movements(map_win, stat_win, &game_map, &dest);
                    break;
                }
                case 'd':
                {
                    dest.x = game_map.hero->pos.x + 1;
                    dest.y = game_map.hero->pos.y;
                    movement_res = handle_movements(map_win, stat_win, &game_map, &dest);
                    break;
                }
                case 's':
                {
                    dest.x = game_map.hero->pos.x;
                    dest.y = game_map.hero->pos.y + 1;
                    movement_res = handle_movements(map_win, stat_win, &game_map, &dest);
                    break;
                }
                case 'a':
                {
                    dest.x = game_map.hero->pos.x - 1;
                    dest.y = game_map.hero->pos.y;
                    movement_res = handle_movements(map_win, stat_win, &game_map, &dest);
                    break;
                }
                default:
                    break;
                }
                // it would be significantly better if we could verify the current position of the hero
                switch (movement_res)
                {
                case MOV_CHEST:
                {
                    // show inventary window -> fixed dimension
                    WINDOW *chest_win = newwin(INV_WIN_HEIGHT,
                                               INV_WIN_WIDTH,
                                               (HEIGHT / 2) - (INV_WIN_HEIGHT / 2),
                                               (WIDTH / 2) - (INV_WIN_WIDTH / 2));
                    show_chest(chest_win, &game_map);
                    getch();
                    delwin(chest_win);
                    wclear(stdscr);
                    refresh();
                    render_map(map_win, &game_map);
                    render_stat_map(stat_win, &game_map, STAT_WIN_WIDTH);
                    break;
                }
                case MOV_PORTAL:
                {
                    // clear();
                    char new_path_map[BUFFERSIZE];
                    strcpy(new_path_map, destination_right_portal(&game_map, &dest));
                    deinit_gmt(&game_map);

                    if (load_game_map(map_win, &game_map, new_path_map, WIDTH, HEIGHT) != 1)
                    {
                        message_dialog("Loading map error", "Could not open map");
                        c = 'q';
                        clear();
                        delwin(map_win);
                        delwin(stat_win);
                        deinit_gmt(&game_map);
                    }
                    else
                    {
                        clear();
                        delwin(map_win);
                        delwin(stat_win);
                        map_win = newwin(game_map.e_height + 2,
                                         game_map.e_width + 2,
                                         ((HEIGHT / 2) - (game_map.e_height / 2)) - 2,
                                         ((WIDTH / 2) - (game_map.e_width / 2)) - (STAT_WIN_WIDTH / 2));
                        stat_win = newwin(game_map.e_height + 2,
                                          STAT_WIN_WIDTH,
                                          ((HEIGHT / 2) - (game_map.e_height / 2)) - 2,
                                          ((WIDTH / 2) + (game_map.e_width / 2)) - (STAT_WIN_WIDTH / 2) + 2);
                        refresh();
                        box(map_win, 0, 0);
                        wrefresh(map_win);
                        render_map(map_win, &game_map);
                        render_stat_map(stat_win, &game_map, STAT_WIN_WIDTH);
                    }

                    break;
                }
                case MOV_DEAD:
                {
                    c = 'q';
                    message_dialog("Lost", "You lost man :(");
                    delwin(map_win);
                    deinit_gmt(&game_map);
                    break;
                }
                case MOV_WIN:
                {
                    if (strcmp(game_map.next_map, "___") == 0)
                    {
                        c = 'q';
                        message_dialog("complimenti!", "sei soppravissuto/a!!!!!!");
                        delwin(map_win);
                        deinit_gmt(&game_map);
                        clear();
                    }
                    else
                    {
                        clear();
                        char new_path_map[BUFFERSIZE];
                        strcpy(new_path_map, game_map.next_map);
                        delwin(map_win);
                        delwin(stat_win);
                        deinit_gmt(&game_map);
                        if (load_game_map(map_win, &game_map, new_path_map, WIDTH, HEIGHT) != 1)
                        {
                            c = 'q';
                        }
                        else
                        {
                            map_win = newwin(game_map.e_height + 2,
                                             game_map.e_width + 2,
                                             ((HEIGHT / 2) - (game_map.e_height / 2)) - 2,
                                             ((WIDTH / 2) - (game_map.e_width / 2)) - (STAT_WIN_WIDTH / 2));
                            stat_win = newwin(game_map.e_height + 2,
                                              STAT_WIN_WIDTH,
                                              ((HEIGHT / 2) - (game_map.e_height / 2)) - 2,
                                              ((WIDTH / 2) + (game_map.e_width / 2)) - (STAT_WIN_WIDTH / 2) + 2);
                            refresh();
                            box(map_win, 0, 0);
                            wrefresh(map_win);
                            render_map(map_win, &game_map);
                            render_stat_map(stat_win, &game_map, STAT_WIN_WIDTH);
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

bool config_file_present(struct dirent *dir)
{
    bool retval = false;
    DIR *subdr = opendir(dir->d_name);
    if (subdr == NULL)
    {
        return retval;
    }
    struct dirent *file;
    // check if in the subdir is present the config.gigi file
    while (((file = readdir(subdr)) != NULL) && !retval)
    {
        if (strcmp(file->d_name, "config.gigi") == 0)
            retval = true;
    }
    closedir(subdr);
    return retval;
}

char *choose_game()
{
    char *retval = (char *)malloc(BUFFERSIZE * sizeof(char));
    strcpy(retval, "data/");
    char path[BUFFERSIZE];
    struct dirent *de;
    char directories[MAXIMUM_GAMES][BUFFERSIZE];
    int dim_dir = 0;
    DIR *dr = opendir("data/");
    if (dr == NULL)
    {
        free(retval);
        return NULL;
    }
    chdir("data/");

    while ((de = readdir(dr)) != NULL)
    {
        if ((strcmp(de->d_name, "..") != 0) &&
            (strcmp(de->d_name, ".") != 0) &&
            (de->d_type == DT_DIR) &&
            (config_file_present(de)) &&
            (dim_dir < MAXIMUM_GAMES))
        {
            strcpy(directories[dim_dir], de->d_name);
            ++dim_dir;
        }
    }
    closedir(dr);
    chdir("..");
    int chosen_index = choose_index(directories, dim_dir);
    if (chosen_index == -1)
    {
        free(retval);
        return NULL;
    }
    else
    {
        strcat(retval, directories[chosen_index]);
        strcat(retval, "/config.gigi");
        return retval;
    }
}

void main_screen(const int WIDTH, const int HEIGHT)
{
    char c;
    do
    {
        noecho();
        clear();
        c = render_main_screen(WIDTH, HEIGHT);
        switch (c)
        {
        case 'l':
        {
            char *path = choose_game();
            if (path != NULL)
            {
                int res = game_loop(path, WIDTH, HEIGHT);
                if (res == -1)
                {
                    message_dialog("General errors", "Errors in game loop");
                }
                free(path);
            }
            break;
        }
        case 'q':
            if (!confirmation_dialog("Exit", "Do you really want to exit?"))
            {
                c = ' ';
            }
            break;
        default:
            break;
        }
    } while (c != 'q');
}
