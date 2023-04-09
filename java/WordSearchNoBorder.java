package word-search;
import java.lang.IndexOutOfBoundsException;

public class WordSearchNoBorder extends WordSearch {
	public WordSearchNoBorder(int num_rows, int num_cols) {
		super(num_rows, num_cols);
		for (int i = 0; i < num_rows * num_cols; i++) {
			data[i] = '.';
		}
		colors = new byte[3 * num_rows * num_cols];
		for (int i = 0; i < colors.length; i++) {
			colors[i] = 64;
		}
	}

	public char get(int i, int j) {
		if (i < 0 || i >= num_rows || j < 0 || j >= num_cols) {
			throw new IndexOutOfBoundsException("ERROR: Trying to access characters outside of valid range in word search.");
		}
		return data[i * num_cols + j];
	}

	public void set(int i, int j, char c) {
		if (i < 0 || i >= num_rows || j < 0 || j >= num_cols) {
			throw new IndexOutOfBoundsException("ERROR: Trying to access characters outside of valid range in word search.");
		}
		data[i * num_cols + j] = c;
	}

	public byte get_r(int i, int j) {
		return colors[3 * (i * num_cols + j) + 0];
	}

	public byte get_g(int i, int j) {
		return colors[3 * (i * num_cols + j) + 1];
	}

	public byte get_b(int i, int j) {
		return colors[3 * (i * num_cols + j) + 2];
	}

	public void set_color(int i, int j, byte c[]) {
		if (i < 0 || i >= num_rows || j < 0 || j >= num_cols) {
			throw new IndexOutOfBoundsException("ERROR: Trying to access colors outside of valid range in word search.");
		}
		if (c.length != 3) {
			throw new IndexOutOfBoundsException("ERROR: Colors have three bytes");
		}
		colors[3 * (i * num_cols + j) + 0] = c[0];
		colors[3 * (i * num_cols + j) + 1] = c[1];
		colors[3 * (i * num_cols + j) + 2] = c[2];
	}

	public void print() {
		for (int row = 0; row < num_rows; row++) {
			for (int col = 0; col < num_cols; col++) {
				System.out.printf(
					" \033[38;2;%d;%d;%dm%s\033[m",
					0xFF & get_r(row, col),
					0xFF & get_g(row, col),
					0xFF & get_b(row, col),
					get(row, col)
				);
			}
			System.out.println();
		}
		System.out.println("\033[m");
	}

	private byte colors[];
}
