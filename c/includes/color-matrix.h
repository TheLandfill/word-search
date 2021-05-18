#pragma once
#include "xorshift.h"

struct Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

struct Color_Matrix {
    struct Color * mat;
    int num_rows;
    int num_cols;
};

static const struct Color default_color = { 64, 64, 64 };

struct Color gen_rand_color(struct xorshift32_state* state);
struct Color * cm_get(struct Color_Matrix * m, int i, int j);