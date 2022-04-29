#include <ncurses.h>
#include <stdio.h>
#include "effects.h"

int effect1(WINDOW *win, size_t time)
{
    int retval = 0;
    int WIDTH, HEIGHT;
    getmaxyx(stdscr, HEIGHT, WIDTH);

    return retval;
}

int title(WINDOW *win, size_t time)
{
    int retval = 0;
    /* const char TITLE[] = "\n\
     ▄▄▄▄▄▄▄ ▄▄▄ ▄▄▄▄▄▄      ▄▄▄▄▄▄▄ ▄▄▄▄▄▄▄ ▄▄▄▄▄▄▄ ▄▄▄▄▄▄▄ \n\
█       █   █   ▄  █    █       █       █       █       █ \n\
█  ▄▄▄▄▄█   █  █ █ █    █   ▄▄▄▄█   ▄   █   ▄   █▄     ▄█ \n\
█ █▄▄▄▄▄█   █   █▄▄█▄   █  █  ▄▄█  █ █  █  █ █  █ █   █   \n\
█▄▄▄▄▄  █   █    ▄▄  █  █  █ █  █  █▄█  █  █▄█  █ █   █  \n\
 ▄▄▄▄▄█ █   █   █  █ █  █  █▄▄█ █       █       █ █   █  \n\
█▄▄▄▄▄▄▄█▄▄▄█▄▄▄█  █▄█  █▄▄▄▄▄▄▄█▄▄▄▄▄▄▄█▄▄▄▄▄▄▄█ █▄▄▄█  \n\
\n\
    "; */

    const char TITLE[] = " \n\
               \\o/                                                           o\n\
          __|__                                                         <|>    \n\
                                                                        /|     \n\
   __o__    o//  \\o__ __o         \\o__ __o/   |o__ __o/    |o__ __o/     |     \n\
  />  \\    |     |     |>        /      |    /       \\    /       \\     o__/_ \n\
  \\o       |\\   / \\   //        />     / \\  />       /\\  />       /\\    |     \n\
   \\       o/   \\o/             \\      \\o/  \\         /  \\         /    |     \n\
   <\\      |     |               o      |    o       o    o       o     o     \n\
o__</     / \\   / \\              <\\__  < >   /\\__ __/\\    /\\__ __/\\     <\\__  \n\
                                         |          \n\
                                 o__     o  \n\
                                 <\\__ __/>";

    const int INITIAL_DEPTH = 2;
    int line = 0;
    int depth = INITIAL_DEPTH;
    char tmp_ch[2];
    for (int i = 0; TITLE[i] != '\0'; ++i)
    {
        if (TITLE[i] == '\n')
        {
            ++line;
            depth = INITIAL_DEPTH;
        }
        else
        {
            tmp_ch[0] = TITLE[i];
            tmp_ch[1] = '\0';
            mvwprintw(win, line, ++depth, tmp_ch);
        }
    }

    return 0;
}