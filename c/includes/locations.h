#pragma once
#include <stddef.h>

struct Coords {
    int row;
    int col;
};

struct Locations {
    struct Coords * list;
    int size;
    struct Coords * back;
};

static const struct Locations default_location = { NULL, 0, NULL };

int allocate_character_hash(struct Locations* char_hash);
void free_character_hash(struct Locations* char_hash);