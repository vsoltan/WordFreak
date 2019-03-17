
#include "readparse.h"

// returns the inputed character in lowercase if passed in uppercase
static char to_lowercase(char c) {
	char result = c;
	if ('A' <= c && c <= 'Z') {
		result = c + 'a' - 'A';
	}
	return result;
}

/*
 * function: read_parse
 * --------------------
 * reads all the files in fd_list, parsing each word, and adding it to a hashmap
 *		using increment_entry to increase its frequency count
 *
 * vars:
 *      buffer       : buffer used by read()
 *      word         : a buffer for building words
 *		hyphen_carry : used to differentiate between "-" and "--"
 *      num_read     : number of chars put into the buffer by read
 *	    total        : total number of chars read
 *	    buff_num     : the number of buffers that have been iterated through
 *		stunt        : used for determining if the char is the last one in the file
 *
 * returns : a hasmap that holds all words parsed and the amount of occurences
 * type	   : (Hashmap *)
 */

Hashmap *read_parse(int num_files, int *fd_list) {
	// hashmap to hold all words
	Hashmap *word_map = init_hashmap();

	char buffer[BUFF_SIZE] = "\0";
	char word[MAX_WORD_SIZE] = "";
	bool hyphen_carry = false;

	int num_read = 0, total = 0, buff_num = 0, stunt = 0;

	// iterates over each file
	for (int cur_file = 0; cur_file < num_files; ++buff_num) {
		num_read = read(fd_list[cur_file], buffer, BUFF_SIZE);
		// if read is unsuccessful
		if (num_read == NO_FILE_READ) {
			perror("read");
			exit(EXIT_FAILURE);
		}
		// if the number of chars read is less than BUFF_SIZE then read() reached the EOF
		if (num_read < BUFF_SIZE) {
			int cf = close(fd_list[cur_file]);
			// confirm that there were no errors closing the file
			if (cf == CLOSE_FAILED) {
				perror("close");
				exit(EXIT_FAILURE);
			}
			++cur_file;
		}

		/*
		 * cur_char: the current character that we are looking at
		 * word_char: the current character in the word that we are building
		 */

		int word_char = 0;
		// iterates through the buffer, constructing the word based on certain specifications
		for(int cur_char = 0; cur_char < num_read; ++cur_char) {
			if (IS_ALPHANUMERIC(buffer[cur_char])) {
				// if the hyphen was not matched with another hyphen then add it to the current word
				if (hyphen_carry) {
					word[word_char++] = '-';
					hyphen_carry = false;
				}
				// lowercase and uppercase variations of the same word are treated equally
				word[word_char++] = to_lowercase(buffer[cur_char]);

				// if there is whitespace or a double-hyphen then the word is done being built
			} else if (IS_WHITESPACE(buffer[cur_char]) || (buffer[cur_char] == '-' && hyphen_carry)) {
				// if no word is constructed keep iterating
				if (strcmp(word, "") != 0) {
					// adds a null char to signify end of word
					word[word_char] = '\0';
					// reset the index for word building
					word_char = 0;
					// add the word to the hashmap
					increment_entry(&word_map, word);
					// reset the word C string
					word[0] = '\0';
				}
			}
			// handles double-hyphens
			if (buffer[cur_char] == '-') {
				hyphen_carry =!hyphen_carry;
			}
		}

		/*
		 * checks if we just read the last word in the file
		 * adds that word to the hashmap, it might get ignored otherwise
		 * typically we ignore words without a whitespace after it because
		 * it got caught between buffers
		 */

		if (stunt < cur_file && strcmp(word, "") != 0) {
			word[word_char] = '\0';
			increment_entry(&word_map, word);
		}
	}
	// we are done with every file (i.e. every file is closed) so we can free the fd_list
	free(fd_list);
	return word_map;
}
