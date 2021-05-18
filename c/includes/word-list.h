#pragma once
#include <stddef.h>

struct Word_List {
    char * word_list;
    int max_word_length;
    int num_words;
};

static const struct Word_List default_word_list = { NULL, 0, 0 };

char * wl_get(struct Word_List* wl, int n);
void wl_allocate(struct Word_List* wl);