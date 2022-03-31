#include <ncurses.h>
#include "aux.h"
#include "data.h"
#include "parser.h"
#include <string.h>

#define TEST_MEM FALSE

void render_pixel(WINDOW *win, unsigned char c, int width, int height)
{
    // switch type
    switch (c)
    {
    case WALL_T:
    {
        mvwprintw(win, height + 1, width + 1, "#");
        break;
    }
    case BOUNCING_T:
    {
        mvwprintw(win, height + 1, width + 1, "=");
        break;
    }
    case TRAP_T:
    {
        mvwprintw(win, height + 1, width + 1, "&");
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
    // pars the file into the map object
    //
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
    /*
    INITIALIZATION IN THE PARSER
    if (init_gmt(&map, width, height) == -1)
    {
        wlog("Game initialization", "Failed initialization of map\n");
        return -1;
    } */
    return pars_map_res;
}

int game_loop(const char *path, int WIDTH, int HEIGHT)
{
    clear();
    config_t config;
    int p_conf_res = load_config(path, &config);
    if (p_conf_res != 1)
    {
        game_map_t game_map;
        FILE *game_file = fopen(config.path_initial_map, "r");
        if(game_file == NULL) 
        {
            wlog("File opening", "Failed to open game_file from path of config type");
            return -1;
        }
        int pars_map_res = load_game_map_s(game_file, &game_map);
        fclose(game_file);

        if (pars_map_res != 1)
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
            char path[BUFFER_LEN];
            load_map_option(path);
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

#if !TEST_MEM
int main()
{
    initscr();
    curs_set(0);
    game_map_t map_game;
    int WIDTH, HEIGHT;
    getmaxyx(stdscr, HEIGHT, WIDTH);
    // printw("WIDTH=%d, HEIGHT=%d", WIDTH, HEIGHT);
    main_screen(WIDTH, HEIGHT);
    // getch();
    endwin();
    return 0;
}

#else

int main()
{
    game_map_t map;
    int width = 6;
    int height = 4;
    char c = 'a';
    while (c != 'q')
    {
        c = (char)getchar();
        printf("Passaggio!\n");
        if (init_gmt(&map, width, height) == -1)
        {
            wlog("Failed initialization of map\n");
            return -1;
        }
        printf("Allocazione!\n");
        deinit_gmt(&map, width, height);
        printf("Deallocazione!\n");
    }
    return 0;
}

#endif
