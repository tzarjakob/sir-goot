#include <ncurses.h>
#include <string.h>
#include "game_logic.h"

int main()
{
    initscr();
    curs_set(0);
    int WIDTH, HEIGHT;
    getmaxyx(stdscr, HEIGHT, WIDTH);
    main_screen(WIDTH, HEIGHT);
    endwin();

    return 0;
}