#include "xorshift.h"

uint32_t xorshift32(struct xorshift32_state *state)
{
	/* Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs" */
	uint32_t x = state->a;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	return state->a = x;
}

float rand_float(struct xorshift32_state * state)
{
    uint32_t cur_value = xorshift32(state);
    uint32_t max_int = (uint32_t)-1;
    return (float)cur_value / (float)max_int;
};