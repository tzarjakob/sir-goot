#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parser.h"
#include "data.h"

int get_next_number(FILE *src)
{
    int result = 0;

    char c_curr;
    int FOUND = 0;
    while (!FOUND)
    {
        // printf("res = %d\n", result);
        c_curr = (char)fgetc(src);
        if (c_curr == EOF)
        {
            return NUMBER_NOT_FOUND;
        }
        else if (c_curr >= '0' && c_curr <= '9')
        {
            FOUND = 1;
            result += (int)(c_curr - '0');
            // printf("res = %d\n", result);
            // NEW WORD!!!
            int end_of_word = 0;
            while (!end_of_word)
            {
                c_curr = (char)fgetc(src);
                if (c_curr == EOF || c_curr == '\n' || c_curr == '\t' || c_curr == '\r' || c_curr == ' ')
                {
                    // end of numer
                    end_of_word = 1;
                }
                else if (c_curr >= '0' && c_curr <= '9')
                {
                    result = (result * 10) + ((int)c_curr - '0');
                    // printf("res = %d\n", result);
                }
                else
                {
                    // not valid character
                    return NOT_VALID_CHAR;
                }
            }
        }
        else if (c_curr == '\n' || c_curr == '\t' || c_curr == '\r' || c_curr == ' ')
        {
            // PASS THIS CHARACTERS...
        }
        else
        {
            return NOT_VALID_CHAR;
        }
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
        else if ((c_curr >= 'A' && c_curr <= 'Z') || (c_curr >= 'a' && c_curr <= 'z'))
        {
            FOUND = 1;
            // NEW WORD!!!
            // printf("%c", c_curr);
            int end_of_word = 0;
            int len = 0;
            next_word[len] = c_curr;
            ++len;
            while (!end_of_word)
            {
                c_curr = (char)fgetc(src);
                // printf("%c", c_curr);
                if (c_curr == EOF || c_curr == '\n' || c_curr == '\t' || c_curr == '\r' || c_curr == ' ')
                {
                    // printf("\nSIAMO QUA\n");
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
        else if (c_curr == '\n' || c_curr == '\t' || c_curr == '\r' || c_curr == ' ')
        {
            // PASS THIS CHARACTERS...
        }
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
        if (strlen(attr_val) < 2)
        {
            // only the = character is not allowed
            SIGNAL = SYNTAX_ERROR;
        }
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

int parser(FILE *src)
{
    int SIGNAL_ERROR = 0;
    char c_curr;

    while (!SIGNAL_ERROR)
    {
        c_curr = (char)fgetc(src);
        switch (c_curr)
        {
        case EOF:
        {
            SIGNAL_ERROR = BUFFER_END;
            break;
        }
        case ' ':
            // printf("%c", c_curr);
            break;
        case '\n':
            // printf("%c", c_curr);
            break;
        case '\t':
            // printf("%c", c_curr);
            break;
        // CASE COMMENT
        case '(':
        {
            // printf("%c", c_curr);
            // printf("COMMENT\n");
            c_curr = (char)fgetc(src);
            // printf("%c", c_curr);
            if (c_curr != '*')
                SIGNAL_ERROR = SYNTAX_ERROR;
            else
            {
                int EOComm = 0;
                // proceed until end of the comment *)
                while (!EOComm && !SIGNAL_ERROR)
                {
                    c_curr = (char)fgetc(src);
                    // printf("%c", c_curr);
                    if (c_curr == EOF)
                        SIGNAL_ERROR = SYNTAX_ERROR;
                    else if (c_curr == '*')
                    {
                        c_curr = (char)fgetc(src);
                        // printf("%c", c_curr);
                        if (c_curr == ')')
                        {
                            EOComm = 1;
                            // printf("END OF COMMENT");
                        }
                        else if (c_curr == EOF)
                            SIGNAL_ERROR = SYNTAX_ERROR;
                    }
                }
            }
            break;
        }
        case '$':
        {
            // printf("$ DEFINE OBJECT\n");
            // define a new object
            char *ob_name = get_next_word(src);
            // printf("ob_name = %s\n", ob_name);
            if (ob_name == NULL)
            {
                SIGNAL_ERROR = SYNTAX_ERROR;
            }
            else if (strcmp(ob_name, "wall") == 0)
            {
                // printf("\nQUESTO E' UN MURO\n");
                // get attributes
                get_attributes(src, 4);
                free(ob_name);
            }
            else
            {
                SIGNAL_ERROR = SYNTAX_ERROR;
            }

            break;
        }
        default:
        {
            SIGNAL_ERROR = SYNTAX_ERROR;
            break;
        }
        }
    }

    return SIGNAL_ERROR;
}

int parser_map(FILE *src, game_map_t *map)
{
    int SIGNAL_ERROR = 0;
    char c_curr;

    while (!SIGNAL_ERROR)
    {
        c_curr = (char)fgetc(src);
        switch (c_curr)
        {
        case EOF:
        {
            SIGNAL_ERROR = BUFFER_END;
            break;
        }
        case ' ':
            break;
        case '\n':
            break;
        case '\t':
            break;
        // CASE COMMENT
        case '(':
        {
            c_curr = (char)fgetc(src);
            if (c_curr != '*')
                SIGNAL_ERROR = SYNTAX_ERROR;
            else
            {
                int EOComm = 0;
                while (!EOComm && !SIGNAL_ERROR)
                {
                    c_curr = (char)fgetc(src);
                    if (c_curr == EOF)
                        SIGNAL_ERROR = SYNTAX_ERROR;
                    else if (c_curr == '*')
                    {
                        c_curr = (char)fgetc(src);
                        if (c_curr == ')')
                        {
                            EOComm = 1;
                        }
                        else if (c_curr == EOF)
                            SIGNAL_ERROR = SYNTAX_ERROR;
                    }
                }
            }
            break;
        }
        case '$':
        {
            // define a new object
            char *ob_name = get_next_word(src);
            if (ob_name == NULL)
            {
                SIGNAL_ERROR = SYNTAX_ERROR;
            }
            else if (strcmp(ob_name, "dimensions") == 0)
            {
                int width = get_next_number(src);
                int height = get_next_number(src);
                if (width != -1 && height != -1)
                    init_gmt(map, width, height);
                else
                    SIGNAL_ERROR = SYNTAX_ERROR;
            }
            else if (strcmp(ob_name, "wall") == 0)
            {
                int top_left_x = get_next_number(src);
                int top_left_y = get_next_number(src);
                int bottom_right_x = get_next_number(src);
                int bottom_right_y = get_next_number(src);
                if (top_left_x != -1 && top_left_x != -1 && top_left_x != -1 && top_left_x != -1)
                {
                }
                else
                    SIGNAL_ERROR = SYNTAX_ERROR;
            }
            else if (strcmp(ob_name, "start_p") == 0)
            {
                int x = get_next_number(src);
                int y = get_next_number(src);
                if (x != -1 && y != -1)
                {
                }
                else
                    SIGNAL_ERROR = SYNTAX_ERROR;
            }
            else if (strcmp(ob_name, "end_p") == 0)
            {
                int x = get_next_number(src);
                int y = get_next_number(src);
                if (x != -1 && y != -1)
                {
                }
                else
                    SIGNAL_ERROR = SYNTAX_ERROR;
            }
            else
            {
                SIGNAL_ERROR = SYNTAX_ERROR;
            }
            free(ob_name);

            break;
        }
        default:
        {
            SIGNAL_ERROR = SYNTAX_ERROR;
            break;
        }
        }
    }

    return SIGNAL_ERROR;
}

int parser_config_file(FILE *src, config_t *config)
{
    int SIGNAL_ERROR = 0;
    char c_curr;

    while (!SIGNAL_ERROR)
    {
        c_curr = (char)fgetc(src);
        switch (c_curr)
        {
        case EOF:
        {
            SIGNAL_ERROR = BUFFER_END;
            break;
        }
        case ' ':
            break;
        case '\n':
            break;
        case '\t':
            break;
        // CASE COMMENT
        case '(':
        {
            c_curr = (char)fgetc(src);
            if (c_curr != '*')
                SIGNAL_ERROR = SYNTAX_ERROR;
            else
            {
                int EOComm = 0;
                // proceed until end of the comment *)
                while (!EOComm && !SIGNAL_ERROR)
                {
                    c_curr = (char)fgetc(src);
                    if (c_curr == EOF)
                        SIGNAL_ERROR = SYNTAX_ERROR;
                    else if (c_curr == '*')
                    {
                        c_curr = (char)fgetc(src);
                        if (c_curr == ')')
                        {
                            EOComm = 1;
                        }
                        else if (c_curr == EOF)
                            SIGNAL_ERROR = SYNTAX_ERROR;
                    }
                }
            }
            break;
        }
        case '$':
        {
            // define a new object
            char *ob_name = get_next_word(src);
            if (ob_name == NULL)
            {
                SIGNAL_ERROR = SYNTAX_ERROR;
            }
            else if (strcmp(ob_name, "path") == 0)
            {
                char *path = get_next_word(src);
                if (src == NULL)
                {
                    SIGNAL_ERROR = SYNTAX_ERROR;
                }
                else
                {
                    strcpy(config->path_initial_map, path);
                    free(path);
                }
                free(ob_name);
            }
            else
            {
                SIGNAL_ERROR = SYNTAX_ERROR;
            }

            break;
        }
        default:
        {
            SIGNAL_ERROR = SYNTAX_ERROR;
            break;
        }
        }
    }

    return SIGNAL_ERROR;
}
