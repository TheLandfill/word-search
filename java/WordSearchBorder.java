package word-search;
import java.lang.IndexOutOfBoundsException;

public class WordSearchBorder extends WordSearch {
	public WordSearchBorder(int n_rows, int n_cols) {
		super(n_rows + 2, n_cols + 2);
		for (int col = 0; col < num_cols; col++) {
			data[get_raw_index(0, col)] = '@';
			data[get_raw_index(num_rows - 1, col)] = '@';
		}
		for (int row = 1; row < num_rows - 1; row++) {
			data[get_raw_index(row, 0)] = '@';
			data[get_raw_index(row, num_cols - 1)] = '@';
		}
	}

	public char get(int i, int j) {
		if (i < 0 || i >= num_rows || j < 0 || j >= num_cols) {
			throw new IndexOutOfBoundsException("ERROR: Trying to access characters outside of valid range in word search.");
		}
		return data[get_index(i, j)];
	}

	public void set(int i, int j, char c) {
		data[get_index(i, j)] = c;
	}

	public char get_raw(int i, int j) {
		if (i < 0 || i >= num_rows || j < 0 || j >= num_cols) {
			throw new IndexOutOfBoundsException("ERROR: Trying to access characters outside of valid range in word search.");
		}
		return data[get_raw_index(i, j)];
	}

	public void print() {
		for (int row = 0; row < num_rows - 2; row++) {
			for (int col = 0; col < num_cols - 2; col++) {
				System.out.print(get(row, col));
			}
			System.out.println();
		}
	}

	private int get_raw_index(int row, int col) {
		return row * num_cols + col;
	}

	private int get_index(int row, int col) {
		return (row + 1) * num_cols + (col + 1);
	}
}
