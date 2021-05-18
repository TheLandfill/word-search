#ifndef WORD_SEARCH_WORD_SEARCH_H
#define WORD_SEARCH_WORD_SEARCH_H
#include <stddef.h>
#include "color-matrix.h"

struct Word_Search {
    char * word_search;
    int num_rows;
    int num_cols;
};

#define WS_EMPTY_CHAR '.'

static const struct Word_Search default_word_search = { NULL, 0, 0 };

char * ws_get(struct Word_Search* ws, int i, int j);
char * ws_get_raw(struct Word_Search* ws, int i, int j);
char * ws_get_no_border(struct Word_Search* ws, int i, int j);

void print_word_search(struct Word_Search* ws);
void print_no_border_word_search(struct Word_Search* ws, struct Color_Matrix* cm);

#endif