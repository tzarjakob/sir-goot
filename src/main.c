#include <ncurses.h>

int main()
{
    initscr();
    char c = getch();
    endwin();
    return 0;
}