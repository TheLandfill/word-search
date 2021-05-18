#include "locations.h"
#include <stdlib.h>

int allocate_character_hash(struct Locations* char_hash) {
    for (int i = 0; i < 26; i++) {
        int size = char_hash[i].size;
        struct Coords * cur_allocation = malloc(sizeof(struct Coords) * size);
        if (cur_allocation == NULL) {
            free_character_hash(char_hash);
            return 0;
        }
        char_hash[i].list = cur_allocation;
        char_hash[i].back = char_hash[i].list;
    }
    return 1;
}

void free_character_hash(struct Locations* char_hash) {
    for (int i = 0; i < 26; i++) {
        free(char_hash[i].list);
    }
}