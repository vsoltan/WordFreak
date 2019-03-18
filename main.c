
#include "readparse.h"

/*
 * function: open_files
 * --------------------
 * dynamically allocates an array to hold the file descriptors
 *		of the passed files contained in file_names
 *
 * returns : an array of all opened file descriptors
 * type	   : (int *)
 */

int *open_files(int num, char **file_names) {
	// list of the file descriptors
	int *fd_list = calloc(num, sizeof(int));
	// checks that calloc was completed successfully
	if (fd_list == MEM_ALLOC_FAIL) {
		// prints out the corresponding error message
		perror("calloc");
		exit(EXIT_FAILURE);
	}
	// iterates through all the files
	for (int i = 0; i < num; ++i) {
		char file_path[256] = PATH;
		// copies the file path into the array of file names
		strcat(file_path, file_names[i]);
		// attempts to open the file, if successful, appends its file descriptor to fd_list
		if ((fd_list[i] = open(file_path, O_RDONLY)) == NO_FILE_OPENED) {
			perror("open");
			return NULL;
		}
	}
	return fd_list;
}

/*
 * function: print_frequency
 * ------------------------
 * iterates through the hashmap and prints out a formatted list of
 *		all the words and their occurences in the processed files
 *
 * returns : void
 */

void print_frequency(Hashmap **map) {
	for (int i = 0; i < HASH_SIZE; ++i) {
		HM_Entry *curr = (*map)->entries[i];
		HM_Entry *curr_next = curr;
		// cannot print out if no entries exist at that particular hash index
		if (curr != NULL) {
			// iterates throughout the linked list and prints out all connected entries
			while (curr_next != NULL) {
				// format the information and write it to STDOUT
				char formatted_line[FORMATTED_LINE_LENGTH] = "";
				sprintf(formatted_line, "%-50s|%10i\n", curr_next->key, curr_next->value);
				// prints out the formatted string in standard output
				int write_num = write(STDOUT, formatted_line, FORMATTED_LINE_LENGTH);
				// error checking
				if (write_num == NO_FILE_WRITE) {
					perror("write");
					exit(EXIT_FAILURE);
				}
				curr_next = curr_next->next;
			}
		}
	}
	// free the hashmap once done
	free_hashmap(map);
}

/*
 * function: process_input
 * ------------------------
 * takes the various forms of users input and converts it into a list of
 *		file descriptors that can be handeled elsewhere
 *
 * vars:
 *		fd_list		:  an array of file descriptors
 *		word_freak	:  the value corresponding to the WORD_FREAK environmental var
 *
 * returns : int *
 */

int *process_input(int argc, char *argv[], int *num_files) {
	// array containing all read file descriptors
	int *fd_list;
	char *word_freak = getenv("WORD_FREAK");

	// env variable handling
	if (word_freak != NULL) {
		// opens the one file referenced by word_freak
		fd_list = open_files((*num_files = 1), &word_freak);
		// if the file "word_freak" doesn't exist
		if (fd_list == FILE_DESC_EMPTY) {
			exit(EXIT_FAILURE);
		}
	}
	// command prompt argument handling
	else if (argc > 1) {
		// gets the file descriptors from the files in the cmd line args, adding them to the list
		fd_list = open_files(argc - 1, argv + 1);
		// null checking
		if(fd_list == FILE_DESC_EMPTY) {
			exit(EXIT_FAILURE);
		}
		// all args will be files except the call to wordfreak
		*num_files = argc - 1;
	}
	// standard input piping handling
	else if (argc == 1) {
		fd_list = malloc(sizeof(int));
		// checks that malloc was completed successfully
		if (fd_list == MEM_ALLOC_FAIL) {
			perror("malloc");
			exit(EXIT_FAILURE);
		}
		// sets the list of file descriptors to standard input
		fd_list[0] = STDIN_FILENO;
		// will only be writing to stdin
		*num_files = 1;
	}
	return fd_list;
}

/*
 * function: run_word_freak
 * ------------------------
 * runs the program by getting the user input, reading and parsing the files
 *		specified by the user, and writing the result to the console
 *
 * vars:
 *		fd_list		:  list of all the file descriptors
 *		num_files	:  the number of files in fd_list
 *
 * returns : 0
 * type	   : int
 */

int run_word_freak(int argc, char *argv[]) {
	int *fd_list;
	int num_files;

	// processes the user input (any form of it), forming an array of file descriptors to be read, and the size of that array
	fd_list = process_input(argc, argv, &num_files);

	// reads and parses the text in the passed files, storing the results in a hashmap
	Hashmap *hm = read_parse(num_files, fd_list);
	print_frequency(&hm);
	return 0;
}

/*
 * function: main
 * --------------
 * executes the program by calling run_word_freak
 *
 * returns : 0
 * type	   : int
 */

int main(int argc, char *argv[]) {
	run_word_freak(argc, argv);
	return 0;
}
