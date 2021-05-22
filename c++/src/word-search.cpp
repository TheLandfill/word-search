#include "word-search.hpp"
#include <iostream>

namespace word_search {

Word_Search::Word_Search(uint32_t n_rows, uint32_t n_cols) :
    num_rows(n_rows),
    num_cols(n_cols),
    data(nullptr)
{}

Word_Search::~Word_Search() {
    delete[] data;
}

Word_Search_Border::Word_Search_Border(uint32_t n_rows, uint32_t n_cols) :
    Word_Search(n_rows, n_cols)
{
    data = new char[(num_rows + 2) * (num_cols + 2)]();
}

char * Word_Search_Border::get(uint32_t i, uint32_t j) {
    if (i >= num_rows || j >= num_cols) {
        return nullptr;
    }
    return data + (i + 1) * (2 + num_cols) + j + 1;
}

char * Word_Search_Border::get_raw(uint32_t i, uint32_t j) {
    if (i >= (num_rows + 2) || j >= (num_cols + 2)) {
        return nullptr;
    }
    return data + i * (2 + num_cols) + j;
}

void Word_Search_Border::print() {
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            char c = *get(i, j);
            std::cout << c;
        }
        std::cout << '\n';
    }
}

Word_Search_No_Border::Word_Search_No_Border(uint32_t n_rows, uint32_t n_cols) :
    Word_Search(n_rows, n_cols),
    colors(new Color[num_rows * num_cols])
{
    data = new char[num_rows * num_cols]();
    std::fill(data, data + num_rows * num_cols, '.');
}

char * Word_Search_No_Border::get(uint32_t i, uint32_t j) {
    if (i >= num_rows || j >= num_cols) {
        return nullptr;
    }
    return data + i * num_cols + j;
}

Color * Word_Search_No_Border::get_color(uint32_t i, uint32_t j) {
    if (i >= num_rows || j >= num_cols) {
        return nullptr;
    }
    return colors + i * num_cols + j;
}

void Word_Search_No_Border::print() {
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            Color& cc = *get_color(i, j);
            std::cout << " \x1b[38;2;" << (uint)(cc.r) << ';' << (uint)(cc.g) << ';' << (uint)(cc.b) << 'm';
            char c = *get(i, j);
            std::cout << c;
        }
        std::cout << '\n';
    }
    std::cout << "\x1b[m";
}

Word_Search_No_Border::~Word_Search_No_Border() {
    delete[] colors;
}

}