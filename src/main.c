#include <ncurses.h>
#include "log.h"
#include "data.h"
#include "parser.h"
#include "render.h"
#include <string.h>



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