#include <ncurses.h>
#include <string.h>
#include <stdbool.h>
#include "screen.h"
#include "../log.h"
#include <dirent.h>

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
        mvwprintw(win, height, width, "&");
        wattroff(win, COLOR_PAIR(2));
        break;
    }
    case HERO_ID_T:
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
    case DOORH_T:
    {
        mvwprintw(win, height, width, "_");
        break;
    }
    case DOORV_T:
    {
        mvwprintw(win, height, width, "|");
        break;
    }
    case KEY_T:
    {
        wattron(win, A_BLINK);
        mvwprintw(win, height, width, "?");
        wattroff(win, A_BLINK);
        break;
    }
    case ZOMBIE_T:
    {
        mvwprintw(win, height, width, "Z");
        break;
    }
    case GHOST_T:
    {
        mvwprintw(win, height, width, "G");
        break;
    }
    case GENERAL_CHEST_T:
    {
        mvwprintw(win, height, width, "C");
        break;
    }
    case MAGICAL_CHEST_T:
    {
        mvwprintw(win, height, width, "M");
        break;
    }
    case BED_T:
    {
        mvwprintw(win, height, width, "=");
        break;
    }
    case PORTAL_T:
        mvwprintw(win, height, width, "H");
        break;
    case EMPTY_SPACE_T:
    {
        mvwprintw(win, height, width, " ");
        break;
    }
    default:
    {
        wlog("Rendering pixel", "Unknown value...");
        break;
    }
    }
}

void show_inventory(WINDOW *inv_win, game_map_t *game_map)
{
    refresh();
    wbkgd(inv_win, COLOR_PAIR(1));
    box(inv_win, 0, 0);
    int line = 1;
    mvwprintw_center(inv_win, line++, INV_WIN_WIDTH, "INVENTORY");
    wrefresh(inv_win);
}

void show_chest(WINDOW *chest_win, game_map_t *game_map)
{
    refresh();
    wbkgd(chest_win, COLOR_PAIR(1));
    box(chest_win, 0, 0);
    int line = 1;
    mvwprintw_center(chest_win, line++, INV_WIN_WIDTH, "CHEST VIEW");
    wrefresh(chest_win);
}

void render_hero(WINDOW *map_win, hero_t *hero)
{
    render_pixel(map_win, HERO_ID_T, hero->pos.x, hero->pos.y);
    wrefresh(map_win);
}
void clear_hero_render(WINDOW *map_win, hero_t *hero)
{
    render_pixel(map_win, EMPTY_SPACE_T, hero->pos.x, hero->pos.y);
    // wrefresh(map_win);
}

void render_stat_map(WINDOW *stat_win, game_map_t *game_map, int width)
{
    wclear(stat_win);
    wbkgd(stat_win, COLOR_PAIR(1));
    box(stat_win, 0, 0);
    // wrefresh(stat_win);
    int line = 1;
    mvwprintw_center(stat_win, line++, width, "STATISTICS");
    int lives = game_map->hero->lives;
    int keys = game_map->hero->keys;
    int exp = game_map->hero->experience;
    line++;
    mvwprintw(stat_win, line++, 3, "Lives: \t %d", lives);
    mvwprintw(stat_win, line++, 3, "Keys: \t %d", keys);
    mvwprintw(stat_win, line++, 3, "Experience: \t %d", exp);
    wrefresh(stat_win);
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

int choose_index(char dirs[MAXIMUM_GAMES][BUFFERSIZE], int n_choices)
{
    int WIDTH, HEIGHT;
    getmaxyx(stdscr, HEIGHT, WIDTH);
    int win_height = HEIGHT / 2;
    int win_width = (WIDTH * 6) / 10;
    WINDOW *choose_game_win = newwin(win_height, win_width,
                                     HEIGHT / 5, WIDTH / 5);
    wbkgd(choose_game_win, COLOR_PAIR(1));

    int c;
    int current_choice = 0;
    keypad(stdscr, true);
    // keypad(choose_game_win, true);
    bool end = false;
    do
    {
        box(choose_game_win, 0, 0);
        int line = 1;
        mvwprintw_center(choose_game_win, line++, win_width, "CHOOSE THE GAME");
        line++;

        for (int res_index = 0; res_index < n_choices; res_index++)
        {
            if (res_index == current_choice)
            {
                wattrset(choose_game_win, COLOR_PAIR(2));
                mvwprintw_center(choose_game_win, line++, win_width, dirs[res_index]);
                wattrset(choose_game_win, COLOR_PAIR(1));
            }
            else
            {
                mvwprintw_center(choose_game_win, line++, win_width, dirs[res_index]);
                // wrefresh(choose_game_win);
            }
            wrefresh(choose_game_win);
        }
        c = getch();
        switch (c)
        {
        case KEY_DOWN:
        {
            current_choice = (current_choice + 1) % n_choices;
            break;
        }
        case KEY_UP:
        {
            current_choice = (current_choice + n_choices - 1) % n_choices;
            break;
        }
        case (int)'\n':
        {
            end = true;
            break;
        }
        case (int)'q':
        {
            current_choice = -1;
            end = true;
            break;
        }
        default:
        {
            break;
        }
        }

    } while (!end);

    delwin(choose_game_win);

    return current_choice;
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
