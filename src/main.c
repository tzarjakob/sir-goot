#include <ncurses.h>
#include "log.h"
#include "data.h"
#include "parser.h"
#include "render.h"
#include <string.h>

#define TEST_MEM FALSE


#if !TEST_MEM
int main()
{
    initscr();
    curs_set(0);
    game_map_t map_game;
    int WIDTH, HEIGHT;
    getmaxyx(stdscr, HEIGHT, WIDTH);
    main_screen(WIDTH, HEIGHT);
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
