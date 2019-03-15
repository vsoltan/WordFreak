
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
#define NO_FILE_OPENED -1
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
			// hashmap thing
			// printf("this is what is getting added to map: %s", word);
			set_entry(&word_map, word);
			// printf("did it change after getting added? %s", word);
			word[0] = '\0';
		}
	}
	// HM_Entry *ret = get_entry(word_map, "tiger");
	// printf("%i\n\n", (ret)?ret->value:0);
	return word_map;
}

// prints out all the words in the read files and their occurences
void print_word_occ(Hashmap *map) {
	// for (int i  = 0; i < 2; ++i) {
	// 	char formatted_line[80] = "";
	// 	// sprintf()
	// 	// write(STDOUT, get_entry(), strlen(get_entry(map, i)));
	for (int i = 0; i < HASH_SIZE; ++i) {
		HM_Entry *curr = map->entries[i];
		if (curr != NULL) {
			printf("%20s  |  %2i\n", curr->key, curr->value);
		}
}

		// printf("%20s  |  %2i\n", get_entry(map, hard_coded_strings[i])->key, get_entry(map, hard_coded_strings[i])->value);
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
	// Hashmap *test_map = init_hashmap();
	// // int test1 = 5;
	// // int test2 = 15;
	// //
	// // DEBUGGING:
	// printf("calling set entry\n");
	// HM_Entry *entry1 = set_entry(&test_map, "Deniz");
	// HM_Entry *retrived1 = get_entry(test_map, "Deniz");
	// printf("%i\n", retrived1->value);
	// HM_Entry *entry2 = set_entry(&test_map, "Valeriy");
	// HM_Entry *retrived2 = get_entry(test_map, "Valeriy");
	// printf("%i\n", retrived2->value);
	// HM_Entry *entry3 = set_entry(&test_map, "Valeriy");
	// printf("calling get entry\n");
	// HM_Entry *retrived3 = get_entry(test_map, "Valeriy");
	// printf("%i\n", retrived3->value);

	int *fd_list;
	// char all_text[][BUFF_SIZE];
	char *all_text;

	// switch statement to handle different numbers of input
	switch(argc) {
	// too few inputs
	case 1:
		printf("fail\n");
		exit(EXIT_FAILURE);
	default:
		printf("test %i\n", argc - 1);
		// int *fd_list = open_files(argc - 1, argv + 1);
		if ((fd_list = open_files(argc - 1, argv + 1)) == NULL) {
			printf("fail\n");
			exit(EXIT_FAILURE);
		}
		// read(fd_list[0], all_text, BUFF_SIZE);
		all_text = read_files(argc - 1, fd_list);
		Hashmap *hm = parse_string(all_text);
		print_word_occ(hm);

	}
	//
	// for (int i = 0; i < BUFF_SIZE; ++i) {
	printf("%s\n", all_text);
	return 0;
}
