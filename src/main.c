#include <ncurses.h>
#include "aux.h"
#include "data.h"

#define TEST_MEM FALSE

void render_pixel(WINDOW *win, unsigned char c, int width, int height)
{
    // switch type
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
    refresh();
}

int game_loop(int WIDTH, int HEIGHT)
{
    clear();
    game_map_t map;
    int width = 6;
    int height = 4;
    WINDOW *map_win = newwin(height+1, width+1, ((HEIGHT/2) - (height/2)), ((WIDTH/2) - (width/2)));
    box(map_win, 0, 0);
    wrefresh(map_win);
    refresh();
    if (init_gmt(&map, width, height) == -1)
    {
        wlog("Failed initialization of map\n");
        return -1;
    }
    // pars the file into the map object
    //
    map.data[0][2] = WALL_T;
    map.data[0][3] = WALL_T;
    map.data[0][4] = WALL_T;
    map.data[1][4] = WALL_T;
    map.data[2][4] = WALL_T;
    map.data[3][4] = WALL_T;
    map.data[3][3] = BOUNCING_T;
    map.data[2][3] = BOUNCING_T;

    char c;
    do
    {
        // render_map(map_win, &map);
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
    deinit_gmt(&map, width, height);
}

// receives as input the specific game to play and tells the function game_loop what is the first map to load
int load_map_option(int WIDTH, int HEIGHT)
{
    int res = game_loop(WIDTH, HEIGHT);
    return res;
}

void render_main_screen(const int WIDTH, const int HEIGHT)
{
    int line = 4;
    mvwprintw_center(stdscr, line++, WIDTH, "This is the main screen");
    mvwprintw_center(stdscr, line++, WIDTH, "Press a to load a game");
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
            if (load_map_option(WIDTH, HEIGHT) == -1)
            {
                wlog("Error in initializing the game\n");
            }
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
