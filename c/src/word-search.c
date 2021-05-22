#include "word-search.h"
#include <stdio.h>

char * ws_get(struct Word_Search* ws, int i, int j) {
    if (i < 0 || i >= ws->num_rows || j < 0 || j >= ws->num_cols) {
        return NULL;
    }
    return ws->word_search + (i + 1) * (2 + ws->num_cols) + j + 1;
}

char * ws_get_raw(struct Word_Search* ws, int i, int j) {
    if (i < 0 || i >= (ws->num_rows + 2) || j < 0 || j >= (ws->num_cols + 2)) {
        return NULL;
    }
    return ws->word_search + i * (ws->num_cols + 2) + j;
}

char * ws_get_no_border(struct Word_Search* ws, int i, int j) {
    if (i < 0 || i >= ws->num_rows || j < 0 || j >= ws->num_cols) {
        return NULL;
    }
    return ws->word_search + i * (ws->num_cols) + j;
}


void print_no_border_word_search(struct Word_Search* ws, struct Color_Matrix * cm) {
    for (int i = 0; i < ws->num_rows; i++) {
        for (int j = 0; j < ws->num_cols; j++) {
            char cur_char = *ws_get_no_border(ws, i, j);
            struct Color * cc = cm_get(cm, i, j);
            printf(" \x1b[38;2;%d;%d;%dm", cc->r, cc->g, cc->b);
            putc(cur_char, stdout);
        }
        putc('\n', stdout);
    }
    printf("\x1b[m");
}

void print_word_search(struct Word_Search* ws) {
    for (int i = 0; i < ws->num_rows; i++) {
        for (int j = 0; j < ws->num_cols; j++) {
            putc(*ws_get(ws, i, j), stdout);
        }
        putc('\n', stdout);
    }
}
