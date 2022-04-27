#include <ncurses.h>
#include <string.h>
#include <assert.h>
#include "game_logic.h"
#include "data.h"

int main()
{
    initscr();
    curs_set(0);
    int WIDTH, HEIGHT;
    getmaxyx(stdscr, HEIGHT, WIDTH);
    if ((MIN_HEIGHT > HEIGHT) || (MIN_WIDTH > WIDTH))
    {
        endwin();
        fprintf(stderr, "Terminal is too small: it is required minimum width=%d and height=%d", MIN_WIDTH, MIN_HEIGHT);
    }
    else
    {
        start_color();
        init_pair(BLACK_WHITE_THEME, COLOR_BLACK, COLOR_WHITE);
        init_pair(WHITE_BLACK_THEME, COLOR_WHITE, COLOR_BLACK);
        init_pair(BLUE, COLOR_WHITE, COLOR_BLUE);
        main_screen();
        endwin();
    }

    return 0;
}