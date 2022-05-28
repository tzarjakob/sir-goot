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
        fprintf(stderr, "Terminal is too small: it is required minimum width=%d and height=%d\n", MIN_WIDTH, MIN_HEIGHT);
    }
    else
    {
        start_color();
        init_pair(MAIN_THEME, COLOR_BLACK, COLOR_YELLOW);
        init_pair(SECONDARY_THEME, COLOR_BLACK, COLOR_GREEN);
        init_pair(MAIN_SCREEN_THEME, COLOR_BLACK, COLOR_WHITE);
        main_screen();
        endwin();
    }

    return 0;
}