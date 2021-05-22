#include "xorshift.hpp"
#include "word-search.hpp"
#include "color.hpp"
#include <ctime>
#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <algorithm>
#include <chrono>

const uint NUM_LETTERS = 26;

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

struct Coords {
    uint32_t row = 0;
    uint32_t col = 0;
    Coords(const uint32_t& a, const uint32_t& b) : row(a), col(b) {}
};

int main(int argc, char ** argv) {
    SETUP_ANSI_TERMINAL
    using hrc = std::chrono::high_resolution_clock;
    auto start = hrc::now();
    using namespace word_search;
    Xorshift32 rng{(uint32_t)time(NULL) | 1};
    if (argc < 2) {
        std::cerr << "Missing filename!\nSyntax: ./word-search file\n";
        return 1;
    }
    std::ifstream reader{argv[1]};
    if (!reader.good()) {
        std::cerr << "Could not read file\n\t`" << argv[1] << "`\n";
        return 1;
    }
    uint32_t num_rows, num_cols;
    reader >> num_rows >> num_cols;
    if (!reader.good()) {
        std::cerr << "Improper File Format! Could not read number of rows or could not read number of columns.\n";
        return 1;
    }
    std::cout << "Solving a " << num_rows << " by " << num_cols << " word search!\n";
    Word_Search_Border ws{num_rows, num_cols};
    std::array<std::vector<Coords>, NUM_LETTERS> character_hash;
    for (auto& cur_letter : character_hash) {
        cur_letter.reserve(num_rows * num_cols / 8);
        cur_letter.clear();
    }
    for (uint32_t i = 0; i < num_rows; i++) {
        for (uint32_t j = 0; j < num_cols; j++) {
            char * cur_char = ws.get(i, j);
            reader >> *cur_char;
            if (!reader.good()) {
                std::cerr << "Improper file format in trying to read element (" << i << ", " << j << ") of the word search!\n";
                return 1;
            }
            if (!isalpha(*cur_char)) {
                std::cerr << "Element `" << *cur_char << "` at (" << i << ", " << j << ") of the word search is not a valid character!\n";
                return 1;
            }
            *cur_char &= ~32;
            character_hash.at((*cur_char) - 'A').push_back(Coords{i, j});
        }
    }

    // ws.print();

    std::vector<std::string> word_list;
    size_t num_words = 0;
    reader >> num_words;
    if (!reader.good()) {
        std::cerr << "Could not read the number of words in the word search!\n";
        return 1;
    }
    word_list.reserve(num_words);
    std::string line;
    std::getline(reader, line);
    for (size_t i = 0; std::getline(reader, line) && i < num_words; i++) {
        line.erase(std::remove_if(line.begin(), line.end(),
            [](const char& c){
                return c == '\n' || c == '\r';
            }
        ), line.end());
        word_list.push_back(line);
    }

    // for (const auto& w : word_list) {
        // std::cout << '`' << w << "`\n";
    // }

    Word_Search_No_Border ws_found{num_rows, num_cols};

    for (const auto& cur_word : word_list) {
        const std::vector<Coords>& cur_first_letter = character_hash[cur_word[0] - 'A'];
        for (const auto& coord : cur_first_letter) {
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    if (dx == 0 && dy == 0) {
                        continue;
                    }
                    int k = 1;
                    Coords cur_letter = coord;
                    cur_letter.row += 1 + dx;
                    cur_letter.col += 1 + dy;
                    while (
                        k < cur_word.length()
                        && ws.get_raw(cur_letter.row, cur_letter.col) != nullptr
                        && cur_word[k] == *ws.get_raw(cur_letter.row, cur_letter.col)
                    ) {
                        cur_letter.row += dx;
                        cur_letter.col += dy;
                        k++;
                    }
                    if (k >= cur_word.length()) {
                        Color cur_color = gen_rand_color(rng);
                        std::cout
                            << '`'
                            << cur_word
                            << "` found @ ("
                            << cur_letter.row - k * dx
                            << ", "
                            << cur_letter.col - k * dy
                            << ") in direction ("
                            << dx
                            << ", "
                            << dy
                            << ")!\n";
                        cur_letter.row--;
                        cur_letter.col--;
                        cur_letter.row -= dx;
                        cur_letter.col -= dy;
                        for (int l = 0; l < k; l++) {
                            *ws_found.get(cur_letter.row, cur_letter.col) = *ws.get(cur_letter.row, cur_letter.col);
                            *ws_found.get_color(cur_letter.row, cur_letter.col) = cur_color;
                            cur_letter.row -= dx;
                            cur_letter.col -= dy;
                        }
                    }
                }
            }
        }
    }

    ws_found.print();
    auto end = hrc::now();
    std::cout << "Took " << std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count() * 1000.0 << " ms\n";
    return 0;
}