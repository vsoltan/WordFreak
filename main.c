
#include "hashmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>

#define PATH "./texts/"
#define BUFF_SIZE 16384
#define MAX_WORD_SIZE 50
#define STDOUT 1
#define NO_FILE_OPENED -1
#define FORMATTED_LINE_LENGTH 62
// checks if the passed char is not a whitespace (space, tab, enter, etc)
#define IS_WHITESPACE(c) c == ' ' || c == '\n' || c == '\t' || c == '\v' || c == '\f' || c == '\r'
// checks if the passed char is alphanumeric
#define IS_ALPHANUMERIC(c) 'a' <= c && c <= 'z' || 'A' <= c && c <= 'Z' || c == '\'' || '0' <= c && c <= '9'

// takes an array file names, and the number of files and opens those files in the directory specified by the constant PATH
// returns an array containing the file descriptors for every file specified


int *open_files(int num, char **file_names) {
	// list of the file descriptors
	int *fd_list = calloc(num, sizeof(int));
	if (fd_list == NULL) {
		perror("calloc");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < num; ++i) {
		char file_path[256] = PATH;
		strcat(file_path, file_names[i]);
		if ((fd_list[i] = open(file_path, O_RDONLY)) == NO_FILE_OPENED) {
			perror("open");
			return NULL;
		}
	}
	return fd_list;
}

char to_lowercase(char c) {
	char result = c;
	if ('A' <= c && c <= 'Z') {
		result = c + 'a' - 'A';
	}
	return result;
}

// combines the functionality of read() and parse()
// allows the program to work with larger files by reading a file in multiple portions so that the buffer doesn't have to be ridiculously large
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
					set_entry(&word_map, word);
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
			set_entry(&word_map, word);
		}

	}
	printf("String terminates at: %i \n", BUFF_SIZE * (buff_num - 1) + num_read);
	free(fd_list);
	return word_map;
}

// prints out all the words in the read files and their occurences
void print_word_occ(Hashmap *map) {
	for (int i = 0; i < HASH_SIZE; ++i) {
		HM_Entry *curr = map->entries[i];
		HM_Entry *curr_next = curr;

		if (curr != NULL) {
			while (curr_next != NULL) {
				char formatted_line[FORMATTED_LINE_LENGTH] = "";
				sprintf(formatted_line, "%-50s|%10i\n", curr_next->key, curr_next->value);
				write(STDOUT, formatted_line, FORMATTED_LINE_LENGTH);
				curr_next = curr_next->next;
			}
		}
	}
}

/*
 * Basic project functionality outline:
 *  - Get user input
 *     - Command line arguments
 *     - Standard input
 *     - Environment variable WORD_FREAK
 *  - Sanitize input
 *  - Iterate over input
 *  - Count each occurence of each word
 *     - Manage each occurence in a hashmap
 */
int main(int argc, char *argv[], char *envp[]) {
	int *fd_list;
	int num_files;
	char *all_text;


	// switch statement to handle different input formats

	// ./wordfreak is the only arg in argv
	if (argc == 1) {
		exit(EXIT_FAILURE);
	}

	// no files were added to fd_list
	if ((fd_list = open_files(argc - 1, argv + 1)) == NULL) {
		exit(EXIT_FAILURE);
	}

	// retriving environmental variables
	if (envp != NULL) {
		printf("envp is not empty!\n");
		char *word_freak = envp[0];
		printf("%s\n", word_freak);
	}

	// piping command case
	if (strcmp(argv[0], "cat") == 0 && (argv[argc - 2], "|") == 0) {
		// TODO: piping???
		/*
		 *      _
		 *  .__(.)<  (MEOW)
		 *   \___)
		 */
		// piping what is
	} else {
		// any other configuration
		num_files = argc - 1;
		// files = fd_list;
	}

	Hashmap *hm = read_parse(num_files, fd_list);
	print_word_occ(hm);

	return 0;
}
