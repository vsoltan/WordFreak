
# include "readparse.h"

static char to_lowercase(char c) {
	char result = c;
	if ('A' <= c && c <= 'Z') {
		result = c + 'a' - 'A';
	}
	return result;
}

Hashmap *read_parse(int num_files, int *fd_list) {
	Hashmap *word_map = init_hashmap();

	char buffer[BUFF_SIZE] = "\0";
	char word[MAX_WORD_SIZE] = "";
	bool hyphen_carry = false;
	// num_read: number of chars put onto the buffer by read()
	// total: total number of chars read
	// buff_num: the number of buffers that have been iterated through
	// stunt: ???
	int num_read = 0, total = 0, buff_num = 0, stunt = 0;

	// iterates over each file
	for (int cur_file = 0; cur_file < num_files; ++buff_num) {
		num_read = read(fd_list[cur_file], buffer, BUFF_SIZE);
		// if the number of chars read is less than the buffer, reached the EOF
		if (num_read < BUFF_SIZE) {
			int cf = close(fd_list[cur_file]);
			if (cf == -1) {
				perror("close");
				exit(EXIT_FAILURE);
			}
			++cur_file;
		}

		// cur_char: the current character that we are looking at
		// word_char: the current character in the word that we are building
		int word_char = 0;
		for(int cur_char = 0; cur_char < num_read; ++cur_char) {
			if (IS_ALPHANUMERIC(buffer[cur_char])) {
				if (hyphen_carry) {
					word[word_char++] = '-';
					hyphen_carry = false;
				}

				word[word_char++] = to_lowercase(buffer[cur_char]);
			} else if (IS_WHITESPACE(buffer[cur_char]) || (buffer[cur_char] == '-' && hyphen_carry)) {
				// if no word is constructed keep iterating
				if (strcmp(word, "") != 0) {
					// reset the word C string
					word[word_char] = '\0';
					// reset the index for word building
					word_char = 0;
					// add the word to the hashmap
					increment_entry(&word_map, word);
					word[0] = '\0';
				}
				// handles double-hyphens
			}
			if (buffer[cur_char] == '-') {
				hyphen_carry = !hyphen_carry;
			}
		}

		// checks if we just read the last word in the file
		// adds that word to the hashmap, it might get ignored otherwise
		// typically we ignore words without a whitespace after it because that means it probably got cut in half
		if (stunt < cur_file && strcmp(word, "") != 0) {
			word[word_char] = '\0';
			increment_entry(&word_map, word);
		}

	}
	free(fd_list);
	return word_map;
}
