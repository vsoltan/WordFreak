
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
#define FORMATTED_LINE_LENGTH 42
// checks if the passed char is not a whitespace (space, tab, enter, etc)
#define IS_WHITESPACE(c) c == ' ' || c == '\n' || c == '\t' || c == '\v' || c == '\f' || c == '\r'
// checks if the passed char is alphanumeric
#define IS_ALPHANUMERIC(c) 'a' <= c && c <= 'z' || 'A' <= c && c <= 'Z' || c == '\'' || c == '-' || '0' <= c && c <= '9'

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

// reads all the passed files
// returns a string containing all the text of all the passed files
char *read_files(int num_files, int *fd_list) {
	char buffer[BUFF_SIZE] = "\0";
	char *text_ptr = (char *) malloc(BUFF_SIZE);
	if (text_ptr == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	int num_read = 0, total = 0, i = 0;

	// iterates the loop until all of the files are read
	for (int cur_file = 0; cur_file < num_files; ++i) {
		num_read = read(fd_list[cur_file], buffer, BUFF_SIZE);
		strcat(text_ptr, buffer);
		buffer[0] = '\0';

		// reached the end of a file
		if (num_read < BUFF_SIZE) {
			int cf = close(fd_list[cur_file]);
			if (cf == -1) {
				perror("close");
				exit(EXIT_FAILURE);
			}
			++cur_file;
		}
		// if there are more files left to read
		if (cur_file < num_files) {
			// increases the size of total
			total = (i + 2) * BUFF_SIZE;

			char tmp_ptr[total];
			tmp_ptr[0] = '\0';
			strcpy(tmp_ptr, text_ptr);
			free(text_ptr);
			text_ptr = (char *) malloc(total);
			strcpy(text_ptr, tmp_ptr);
		}
	}
	text_ptr[BUFF_SIZE * (i - 1) + num_read] = '\0';
	printf("String terminates at: %i \n", BUFF_SIZE * (i - 1) + num_read);
	return text_ptr;
}

char to_lowercase(char c) {
	char result = c;
	if ('A' <= c && c <= 'Z') {
		result = c + 'a' - 'A';
	}
	return result;
}

// input: a string of words to be parsed
// output: a hashmap which
// reads the passed string and counts the number of occurences for each word
// a word is defined as follows:
//  - a set of alphanumeric characters (a-z, A-Z, 0-9), em-dash(-), and apostrophes(')
//  - seperated by whitespace characters (space, tab, enter)
Hashmap *parse_string(char *text) {
	// empty string, nothing to be parsed
	if (text == NULL) {
		return NULL;
	}
	// initialize a hashmap to store all occurances of text
	Hashmap *word_map = init_hashmap();
	// number of characters that have to be parsed
	int length = strlen(text);

	// iterates over the text
	char word[MAX_WORD_SIZE] = "";
	for(int i = 0, j = 0; i < length; ++i) {

		if (IS_ALPHANUMERIC(text[i])) {
			word[j++] = to_lowercase(text[i]);
		}
		if (IS_WHITESPACE(text[i])) {
			if (strcmp(word, "") == 0) continue;
			word[j] = '\0';
			j = 0;
			// printf("trying: %s\n", word);
			set_entry(&word_map, word);
			word[0] = '\0';
		}
	}
	// done with the string so we can free it.
	free(text);
	return word_map;
}

// combines the functionality of read() and parse()
// allows the program to work with larger files by reading a file in multiple portions so that the buffer doesn't have to be ridiculously large
Hashmap *read_parse(int num_files, int *fd_list) {
	Hashmap *word_map = init_hashmap();

	char buffer[BUFF_SIZE] = "\0";
	char word[MAX_WORD_SIZE] = "";
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
				word[word_char++] = to_lowercase(buffer[cur_char]);
			}
			if (IS_WHITESPACE(buffer[cur_char])) {
				// if no word is constructed keep iterating
				if (strcmp(word, "") == 0) continue;
				// reset the word C string
				word[word_char] = '\0';
				// reset the index for word building
				word_char = 0;
				// add the word to the hashmap
				set_entry(&word_map, word);
				word[0] = '\0';
			}
		}

		// TODO: we need a way to check if the word we just read is the last word in the file
		// if that's true, then add it to the hashmap, because normally it would be ignored since there is no space after the last word
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
				sprintf(formatted_line, "%-30s|%10i\n", curr_next->key, curr_next->value);
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
int main(int argc, char *argv[]) {
	int *fd_list;
	char *all_text;

	// switch statement to handle different input formats
	switch(argc) {
	// TODO: piping for when no input is given
	case 1:
		printf("fail\n");
		exit(EXIT_FAILURE);
	default:
		if ((fd_list = open_files(argc - 1, argv + 1)) == NULL) {
			printf("fail\n");
			exit(EXIT_FAILURE);
		}
		// all_text = read_files(argc - 1, fd_list);
		// Hashmap *hm = parse_string(all_text);
		Hashmap *hm = read_parse(argc - 1, fd_list);
		print_word_occ(hm);

	}
	return 0;
}
