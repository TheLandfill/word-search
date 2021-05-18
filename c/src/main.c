#include "word-search.h"
#include "word-list.h"
#include "locations.h"
#include "color-matrix.h"
#include "xorshift.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#define NUM_LETTERS 26

#ifdef WIN32
#include <windows.h>
#define SETUP_ANSI_TERMINAL setup_ansi_terminal();
void setup_ansi_terminal() {
	DWORD l_mode;
	HANDLE hstd = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hstd == INVALID_HANDLE_VALUE) {
		MDXX_warn("Won't be able to display ANSI terminal characters.\n");
		return;
	}
	GetConsoleMode(hstd, &l_mode);
	SetConsoleMode(hstd, l_mode | DISABLE_NEWLINE_AUTO_RETURN | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}
#else
#define SETUP_ANSI_TERMINAL
#endif

int main(int argc, char ** argv) {
    SETUP_ANSI_TERMINAL
    int error_code = 0;
    if (argc < 2) {
        fputs("Missing filename!\nSyntax: ./word-search file", stderr);
        return 1;
    }
    FILE * reader = fopen(argv[1], "r");
    if (reader == NULL) {
        fprintf(stderr, "File `%s` does not exist!", argv[1]);
        return 1;
    }
    struct Word_Search ws = default_word_search;
    struct Word_List wl = default_word_list;
    if (fscanf(reader, "%d %d", &ws.num_rows, &ws.num_cols) < 2) {
        fputs("Improper File Format!", stderr);
        error_code = 1;
        goto close_file;
    }
    printf("Solving a %d by %d word search!\n", ws.num_rows, ws.num_cols);
    ws.word_search = calloc((ws.num_rows + 2) * (ws.num_cols + 2), sizeof(char));
    struct Locations character_hash[NUM_LETTERS];
    for (int i = 0; i < NUM_LETTERS; i++) {
        character_hash[i] = default_location;
    }

    for (int i = 0; i < ws.num_rows; i++) {
        for (int j = 0; j < ws.num_cols; j++) {
            //char * cur_char = word_search + (i * (num_cols + 2)) + j;
            char * cur_char = ws_get(&ws, i, j);
            if (fscanf(reader, " %c", cur_char) == 0) {
                fprintf(stderr, "Improper file format in trying to read element (%d, %d) of the word search!\n", i, j);
                error_code = 1;
                goto clean_up_word_search;
            }
            if (!isalpha(*cur_char)) {
                fprintf(stderr, "Element `%c` at (%d, %d) of the word search is not a valid character!\n", *cur_char, i, j);
                error_code = 1;
                goto clean_up_word_search;
            }
            *cur_char &= ~32;
            character_hash[(*cur_char) - 'A'].size++;
        }
    }

    // print_word_search(&ws);

    if (ws.num_rows < ws.num_cols) {
        wl.max_word_length = ws.num_rows;
    } else {
        wl.max_word_length = ws.num_cols;
    }
    wl.max_word_length += 2;

    if (fscanf(reader, " %d", &wl.num_words) == 0) {
        fputs("Could not read the number of words in the word search!\n", stderr);
        error_code = 1;
        goto clean_up_word_search;
    }

    wl_allocate(&wl);

    if (wl.word_list == NULL) {
        fputs("Could not allocate words_to_find!", stderr);
        error_code = 1;
        goto clean_up_word_search;
    }

    fgets(wl_get(&wl, 0), wl.max_word_length, reader);

    for (int i = 0; i < wl.num_words; i++) {
        char * cur_word = wl_get(&wl, i);
        fgets(cur_word, wl.max_word_length, reader);
        cur_word[strcspn(cur_word, "\r\n")] = '\0';
    }

    // for (int i = 0; i < wl.num_words; i++) {
        // printf("`%s`\n", wl_get(&wl, i));
    // }

    if (!allocate_character_hash(character_hash)) {
        fputs("Could not allocate character hash!", stderr);
        error_code = 1;
        goto clean_up_words_to_find;
    }

    struct Word_Search found_ws = { NULL, ws.num_rows, ws.num_cols };
    found_ws.word_search = malloc(sizeof(char) * ws.num_rows * ws.num_cols);
    if (found_ws.word_search == NULL) {
        fputs("Could not allocate found word search!", stderr);
        error_code = 1;
        goto clean_up_character_hash;
    }

    struct Color_Matrix cm = { NULL, ws.num_rows, ws.num_cols };
    cm.mat = malloc(sizeof(struct Color) * cm.num_rows * cm.num_cols);
    if (cm.mat == NULL) {
        fputs("Could not allocate color matrix!", stderr);
        error_code = 1;
        goto clean_up_found_ws;
    }
    for (int i = 0; i < cm.num_rows; i++) {
        for (int j = 0; j < cm.num_cols; j++) {
            *cm_get(&cm, i, j) = default_color;
        }
    }

    struct xorshift32_state seed = { time(NULL) | 1 };

    memset(found_ws.word_search, WS_EMPTY_CHAR, found_ws.num_rows * found_ws.num_cols);
    
    for (int i = 0; i < ws.num_rows; i++) {
        for (int j = 0; j < ws.num_cols; j++) {
            char cur_char = *ws_get(&ws, i, j);
            struct Coords ** cur_coord = &character_hash[cur_char - 'A'].back;
            (*cur_coord)->row = i;
            (*cur_coord)->col = j;
            (*cur_coord)++;
        }
    }

    for (int i = 0; i < wl.num_words; i++) {
        char * cur_word = wl_get(&wl, i);
        struct Locations * cur_first_letter = character_hash + (cur_word[0] - 'A');
        for (int j = 0; j < cur_first_letter->size; j++) {
            struct Coords * cur_coords = cur_first_letter->list + j;
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <=1; dy++) {
                    if (dx == 0 && dy == 0) {
                        continue;
                    }
                    int k = 1;
                    struct Coords cur_letter = *cur_coords;
                    cur_letter.row += 1 + dx;
                    cur_letter.col += 1 + dy;
                    while (
                        cur_word[k] != '\0'
                        && ws_get_raw(&ws, cur_letter.row, cur_letter.col) != NULL
                        && cur_word[k] == *ws_get_raw(&ws, cur_letter.row, cur_letter.col)
                    ) {
                        cur_letter.row += dx;
                        cur_letter.col += dy;
                        k++;
                    }
                    // printf("%s\n", cur_word + k);
                    if (cur_word[k] == '\0') {
                        struct Color cur_color = gen_rand_color(&seed);
                        printf("`%s` found @ (%d, %d) in direction (%d, %d)!\n",
                            cur_word,
                            cur_letter.row - k * dx,
                            cur_letter.col - k * dy,
                            dx,
                            dy);
                        cur_letter.row--;
                        cur_letter.col--;
                        cur_letter.row -= dx;
                        cur_letter.col -= dy;
                        for (int l = 0; l < k; l++) {
                            char * cur_char_ptr = ws_get(&ws, cur_letter.row, cur_letter.col);
                            *ws_get_no_border(&found_ws, cur_letter.row, cur_letter.col) = *cur_char_ptr;
                            *cm_get(&cm, cur_letter.row, cur_letter.col) = cur_color;
                            cur_letter.row -= dx;
                            cur_letter.col -= dy;
                        }
                        // goto word_found;
                    }
                }
            }
        }
        // word_found:
        // ;
    }

    puts("");
    print_no_border_word_search(&found_ws, &cm);

    clean_up_color_matrix:
        free(cm.mat);
    clean_up_found_ws:
        free(found_ws.word_search);
    clean_up_character_hash:
        free_character_hash(character_hash);
    clean_up_words_to_find:
        free(wl.word_list);
    clean_up_word_search:
        free(ws.word_search);
    close_file:
        fclose(reader);
    return error_code;
}