package word_search;
import java.util.Scanner;
import java.util.InputMismatchException;
import java.io.FileNotFoundException;
import java.io.File;
import java.util.ArrayList;
import java.time.Instant;

class Main {
	public static void main(String args[]) {
		long start = System.nanoTime();
		if (args.length == 0) {
			System.err.println("Missing filename!\nSyntax: java Main file");
			return;
		}
		File word_search_file = null;
		Scanner reader = null;
		try {
			word_search_file = new File(args[0]);
			reader = new Scanner(word_search_file);
		} catch (FileNotFoundException ex) {
			System.err.println("File `" + args[0] + "` could not be opened.");
			return;
		}
		int num_rows, num_cols;
		try {
			num_rows = reader.nextInt();
			num_cols = reader.nextInt();
			reader.nextLine();
		} catch(InputMismatchException ex) {
			System.err.println("Improper File Format! Could not read number of rows or could not read number of columns.");
			return;
		}
		System.out.println("Solving a " + num_rows + " by " + num_cols + " word search!");
		WordSearchBorder wsb = new WordSearchBorder(num_rows, num_cols);
		ArrayList<ArrayList<int[]>> char_hash = new ArrayList<ArrayList<int[]>>();
		for (int i = 0; i < NUM_CHARS; i++) {
			char_hash.add(new ArrayList<int[]>());
		}
		for (int row = 0; row < num_rows; row++) {
			String str = reader.nextLine();
			for (int col = 0; col < num_cols; col++) {
				char c = str.charAt(col);
				if (!Character.isLetter(c)) {
					System.err.println("ERROR: Character `" + c + "` at (" + row + ", " + col + ") is not a valid letter!");
					return;
				}
				c &= ~32;
				wsb.set(row, col, c);
				char_hash.get(c - 'A').add(new int[]{row, col});
			}
		}
		ArrayList<String> word_list = new ArrayList<String>();
		int num_words = 0;
		try {
			num_words = reader.nextInt();
			reader.nextLine();
		} catch(InputMismatchException ex) {
			System.err.println("Could not read the number of words in the word list.");
			return;
		}
		for (int i = 0; i < num_words; i++) {
			word_list.add(reader.nextLine());
		}
		Xorshift rng = new Xorshift((int)(Instant.now().toEpochMilli() | (long)1));
		Color color = new Color();
		WordSearchNoBorder ws_found = new WordSearchNoBorder(num_rows, num_cols);
		for (int i = 0; i < word_list.size() ; i++) {
			String cur_word = word_list.get(i);
			ArrayList<int[]> cur_first_letter = char_hash.get((int)(cur_word.charAt(0) - 'A'));
			for (int j = 0; j < cur_first_letter.size(); j++) {
				int[] coords = cur_first_letter.get(j);
				for (int dx = -1; dx <= 1; dx++) {
					for (int dy = -1; dy <= 1; dy++) {
						if (dx == 0 && dy == 0) {
							continue;
						}
						int[] cur_pos = new int[2];
						cur_pos[0] = coords[0];
						cur_pos[1] = coords[1];
						int k = 1;
						cur_pos[0] += 1 + dx;
						cur_pos[1] += 1 + dy;
						while (
							k < cur_word.length()
							&& wsb.get_raw(cur_pos[0], cur_pos[1]) != '@'
							&& cur_word.charAt(k) == wsb.get_raw(cur_pos[0], cur_pos[1])
						) {
							cur_pos[0] += dx;
							cur_pos[1] += dy;
							k++;
						}
						if (k >= cur_word.length()) {
							byte[] cur_color = color.gen_random_color(rng);
							System.out.println(
								"`"
								+ cur_word
								+ "` found @ ("
								+ (cur_pos[0] - k * dx)
								+ ", "
								+ (cur_pos[1] - k * dy)
								+ ") in direction ("
								+ dx
								+ ", "
								+ dy
								+ ")!"
							);
							cur_pos[0]--;
							cur_pos[1]--;
							cur_pos[0] -= dx;
							cur_pos[1] -= dy;
							for (int l = 0; l < k; l++) {
								ws_found.set(cur_pos[0], cur_pos[1], wsb.get(cur_pos[0], cur_pos[1]));
								ws_found.set_color(cur_pos[0], cur_pos[1], cur_color);
								cur_pos[0] -= dx;
								cur_pos[1] -= dy;
							}
						}
					}
				}
			}
		}

		ws_found.print();
		long end = System.nanoTime();
		System.out.println("Took " + ((end - start) / 1_000_000) + " ms");
	}
	static final int NUM_CHARS = 26;
}
