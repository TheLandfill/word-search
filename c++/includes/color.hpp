#pragma once
#include "xorshift.hpp"

namespace word_search {

struct Color {
    unsigned char r = 64;
    unsigned char g = 64;
    unsigned char b = 64;
};

Color gen_rand_color(Xorshift32& state);

}