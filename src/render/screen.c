#include <ncurses.h>
#include "screen.h"

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
        // wattrset(win, COLOR_PAIR(2));
        wattron(win, A_BOLD);
        mvwprintw(win, height, width, "#");
        wattroff(win, A_BOLD);
        // wattroff(win, COLOR_PAIR(2));
        break;
    }
    case BOUNCING_T:
    {
        mvwprintw(win, height, width, "=");
        break;
    }
    case TRAP_T:
    {
        wattrset(win, COLOR_PAIR(2));
        wattron(win, A_BLINK);
        mvwprintw(win, height, width, "&");
        wattroff(win, A_BLINK);
        wattroff(win, COLOR_PAIR(2));
        break;
    }
    case HERO_P:
    {
        // wattron(win, A_BLINK);
        mvwprintw(win, height, width, "X");
        // wattroff(win, A_BLINK);
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
    wbkgd(win, COLOR_PAIR(1));
    box(win, 0, 0);
    for (int i = 0; i < map->e_height; ++i)
    {
        for (int j = 0; j < map->e_width; ++j)
        {
            render_pixel(win, map->data[i][j], j, i);
        }
    }
    wrefresh(win);
}

void render_main_screen(const int WIDTH, const int HEIGHT)
{
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_YELLOW);
    init_pair(2, COLOR_BLUE, COLOR_YELLOW);
    bkgd(COLOR_PAIR(1));
    int line = 4;
    box(stdscr, 0, 0);
    mvwprintw_center(stdscr, line++, WIDTH, "This is the main screen");
    mvwprintw_center(stdscr, line++, WIDTH, "Press l to load a game");
    mvwprintw_center(stdscr, line++, WIDTH, "Press q to quit");
}
