#include "word-list.h"
#include <stdlib.h>

char * wl_get(struct Word_List* wl, int n) {
    return wl->word_list + n * (wl->max_word_length + 1);
}

void wl_allocate(struct Word_List* wl) {
    wl->word_list = calloc((wl->max_word_length + 1) * wl->num_words, sizeof(char));
}