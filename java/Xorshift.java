package word_search;
public class Xorshift {
	Xorshift(int seed) {
		cur_state = seed;
	}

	int get_rand() {
		cur_state ^= cur_state << 13;
		cur_state ^= cur_state << 17;
		cur_state ^= cur_state >> 5;
		return cur_state;
	}

	float rand_float() {
		double cur_value = get_rand();
		double max_value = 0x00000000FFFFFFFFL;
		float out = (float)(cur_value / max_value);
		return out;
	}

	int cur_state;
}
