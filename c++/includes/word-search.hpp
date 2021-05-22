#pragma once
#include "color.hpp"
#include <cstdint>

namespace word_search {

class Word_Search {
public:
    Word_Search(uint32_t n_rows, uint32_t n_cols);
    virtual char * get(uint32_t i, uint32_t j) = 0;
    virtual void print() = 0;
    ~Word_Search();
protected:
    uint32_t num_rows = 0;
    uint32_t num_cols = 0;
    char * data = nullptr;
};

class Word_Search_Border : public Word_Search {
public:
    Word_Search_Border(uint32_t n_rows, uint32_t n_cols);
    char * get(uint32_t i, uint32_t j) override;
    char * get_raw(uint32_t i, uint32_t j);
    void print() override;
};

class Word_Search_No_Border : public Word_Search {
public:
    Word_Search_No_Border(uint32_t n_rows, uint32_t n_cols);
    char * get(uint32_t i, uint32_t j) override;
    Color * get_color(uint32_t i, uint32_t j);
    void print() override;
    ~Word_Search_No_Border();
private:
    Color * colors;
};

}