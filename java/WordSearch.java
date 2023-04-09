package word_search;
public abstract class WordSearch {
	public WordSearch(int n_rows, int n_cols) {
		num_rows = n_rows;
		num_cols = n_cols;
		data = new char[n_rows * n_cols];
	}

	public abstract char get(int i, int j);
	public abstract void set(int i, int j, char c);

	public abstract void print();

	protected int num_rows, num_cols;
	protected char[] data;
}
