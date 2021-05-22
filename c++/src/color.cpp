#include "color.hpp"

namespace word_search {

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

Color hsl_color(float h, float s, float l) {
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
	typedef unsigned char uc;
    Color final{ (uc)(r * 255), (uc)(g * 255), (uc)(b * 255) };
    return final;
}

Color gen_rand_color(Xorshift32& rng) {
    float hue = rng.rand_float();
    float saturation = rng.rand_float() * 0.2f + 0.8f;
    float lightness = rng.rand_float() * 0.3f + 0.5f;
    Color output = hsl_color(hue, saturation, lightness);
    // printf("Color: #%x%x%x\n", output.r, output.g, output.b);
    return output;
}

}