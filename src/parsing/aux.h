#ifndef AUX_H
#define AUX_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../data.h"
#include "../log.h"

int get_next_number(FILE *src);
char *get_next_word(FILE *src);
int get_attribute(FILE *src);
int get_attributes(FILE *src, int N);
char **get_next_N_word(FILE *src, const int N);

#endif 
