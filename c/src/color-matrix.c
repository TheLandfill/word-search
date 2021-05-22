#include "color-matrix.h"
#include "xorshift.h"
#include <stddef.h>
#include <stdio.h>

float hue2rgb(float p, float q, float h) {
	if (h < 0) {
		h += 1;
	}
	if (h > 1) {
		h -= 1;
	}
	if (6 * h < 1) {
		return p + ((q - p) * 6 * h);
	}
	if (2 * h < 1) {
		return q;
	}
	if (3 * h < 2) {
		return p + ((q - p) * 6 * ((2.0f / 3.0f) - h));
	}
	return p;
}

struct Color hsl_color(float h, float s, float l) {
	float q, p, r, g, b;
	if (s == 0) {
		r = g = b = l; // achromatic
	} else {
		q = l < 0.5 ? (l * (1 + s)) : (l + s - l * s);
		p = 2 * l - q;
		r = hue2rgb(p, q, h + 1.0f / 3);
		g = hue2rgb(p, q, h);
		b = hue2rgb(p, q, h - 1.0f / 3);
	}
    struct Color final = { r * 255, g * 255, b * 255 };
    return final;
}

struct Color gen_rand_color(struct xorshift32_state* state) {
    float hue = rand_float(state);
    float saturation = rand_float(state) * 0.2f + 0.8f;
    float lightness = rand_float(state) * 0.3f + 0.5f;
    struct Color output = hsl_color(hue, saturation, lightness);
    // printf("Color: #%x%x%x\n", output.r, output.g, output.b);
    return output;
}

struct Color * cm_get(struct Color_Matrix* m, int i, int j) {
    if (i < 0 || i >= m->num_rows || j < 0 || j >= m->num_cols) {
       return NULL;
    }
    return m->mat + i * m->num_cols + j;
}