#include <string.h>
#include "log.h"

#include <ncurses.h>


int wlog(const char *topic, const char *text)
{
    FILE *log_file = fopen("log/main.log", "a");
    if (log_file == NULL)
    {
        return -1;
    }
    fprintf(log_file, "LOG on %s: \t %s \n", topic, text);
    fclose(log_file);
    return 0;
}
int wlog_int(const char *topic, int x)
{
    FILE *log_file = fopen("log/main.log", "a");
    if (log_file == NULL)
    {
        return -1;
    }
    fprintf(log_file, "LOG on %s: \t %d \n", topic, x);
    fclose(log_file);
    return 0;
}
