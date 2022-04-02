#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../data.h"
#include "../log.h"

int get_next_number(FILE *src)
{
    int result = 0;

    char c_curr;
    int FOUND = 0;
    while (!FOUND)
    {
        c_curr = (char)fgetc(src);
        if (c_curr == EOF) { return NUMBER_NOT_FOUND; }
        else if (c_curr >= '0' && c_curr <= '9')
        {
            FOUND = 1;
            result += (int)(c_curr - '0');
            int end_of_word = 0;
            while (!end_of_word)
            {
                c_curr = (char)fgetc(src);
                if (c_curr == EOF || c_curr == '\n' || c_curr == '\t' || c_curr == '\r' || c_curr == ' ') { end_of_word = 1; }
                else if (c_curr >= '0' && c_curr <= '9') { result = (result * 10) + ((int)c_curr - '0'); }
                else { return NUMBER_NOT_FOUND; }
            }
        }
        else if (c_curr == '\n' || c_curr == '\t' || c_curr == '\r' || c_curr == ' ') { }
        else { return NUMBER_NOT_FOUND; }
    }
    return result;
}

char *get_next_word(FILE *src)
{
    char *next_word = (char *)malloc(BUFFERSIZE * sizeof(char));
    char c_curr;
    int FOUND = 0;
    while (!FOUND)
    {
        c_curr = (char)fgetc(src);
        if (c_curr == EOF)
        {
            free(next_word);
            return NULL;
        }
        else if ((c_curr >= 'A' && c_curr <= 'Z') ||
                 (c_curr >= 'a' && c_curr <= 'z') || c_curr == '/' ||
                 (c_curr >= '0' && c_curr <= '9') || c_curr == '_')
        {
            FOUND = 1;
            int end_of_word = 0;
            int len = 0;
            next_word[len] = c_curr;
            ++len;
            while (!end_of_word)
            {
                c_curr = (char)fgetc(src);
                if (c_curr == EOF || c_curr == '\n' || c_curr == '\t' || c_curr == '\r' || c_curr == ' ')
                {
                    end_of_word = 1;
                    next_word[len] = '\0';
                    return next_word;
                }
                else
                {
                    if (BUFFERSIZE < len + 2)
                    {
                        // OUT OF RANGE
                        free(next_word);
                        return NULL;
                    }
                    else
                    {
                        next_word[len] = c_curr;
                        len++;
                    }
                }
            }
        }
        else if (c_curr == '\n' || c_curr == '\t' || c_curr == '\r' || c_curr == ' ') { }
        else
        {
            free(next_word);
            return NULL;
        }
    }
    return next_word;
}

int get_attribute(FILE *src)
{
    char *next_word = get_next_word(src);
    int SIGNAL = 0;
    if (next_word == NULL)
    {
        SIGNAL = SYNTAX_ERROR;
    }
    else
    {
        char *attr_val = strchr(next_word, '=');
        // examples of attr_val -> "=321" or "=-21.321" or "=pippo"
        if (strlen(attr_val) < 2) { SIGNAL = SYNTAX_ERROR; }
        else
        {
            attr_val = attr_val + (sizeof(char));
            printf("attr_val=%s\n", attr_val);
            free(next_word);
        }
    }
    return SIGNAL;
}

int get_attributes(FILE *src, int N)
{
    int SIGNAL = 0;
    while (N > 0)
    {
        SIGNAL = get_attribute(src);
        N--;
    }
    return SIGNAL;
}

// get next N word -- return NULL if error
char **get_next_N_word(FILE *src, const int N)
{
    char **res = (char **)malloc(N * (sizeof(char *)));
    char c_curr;
    int SIGNAL_ERROR = 0;
    for (int i = 0; i < N;)
    {
        char *tmp = get_next_word(src);
        if (tmp == NULL)
        {
            printf("\nNULL VALUE, return error\n");
            return NULL;
        }
        else
        {
            res[i] = tmp;
            i++;
        }
    }
    return res;
}
