
#include "readparse.h"

/*
 * function: open_files
 * --------------------
 * dynamically allocates an array to hold the file descriptors
 *      of the passed files contained in file_names
 *
 * returns : an array of all opened file descriptors
 * type	   : (int *)
 */

int *open_files(int num, char **file_names) {
	// list of the file descriptors
	int *fd_list = calloc(num, sizeof(int));
	// checks that calloc was completed successfully
	if (fd_list == NULL) {
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
 * iterates through the hashmap and prints out all the words and their occurences in the processed files
 * returns : void
 */

void print_frequency(Hashmap **map) {
	for (int i = 0; i < HASH_SIZE; ++i) {
		HM_Entry *curr = (*map)->entries[i];
		HM_Entry *curr_next = curr;
		// do we need this if statement?
		if (curr != NULL) {
			while (curr_next != NULL) {
				// format the information and write it to STDOUT
				char formatted_line[FORMATTED_LINE_LENGTH] = "";
				sprintf(formatted_line, "%-50s|%10i\n", curr_next->key, curr_next->value);
				write(STDOUT, formatted_line, FORMATTED_LINE_LENGTH);
				curr_next = curr_next->next;
			}
		}
	}
	// we are done with the hashmap so free it
	free_hashmap(map);
}

// take the cmd parameters from main()
int *process_input(int argc, char *argv[], int *num_files) {
	// array containing all read file descriptors
	// TODO free return_val in main - done
	int *fd_list;
	char *word_freak = getenv("WORD_FREAK");
	char buffer[BUFF_SIZE];

	// ENVIRONMENTAL VARIABLE
	if (word_freak != NULL) {
		fd_list = open_files((*num_files = 1), &word_freak);
		// if the file "word_freak" doesn't exist
		if (fd_list == NULL) {
			exit(EXIT_FAILURE);
		}

	} else if (argc > 1) {
		// CMD ARGUMENTS (Default case)
		// gets all the fd from the files in the cmd line args and adds them to the list
		fd_list = open_files(argc - 1, argv + 1);
		*num_files = argc - 1;
		// null checking
		if(fd_list == NULL) {
			exit(EXIT_FAILURE);
		}
	}
	// standard input read
	else if (argc == 1) {
		fd_list = malloc(sizeof(int));
		if (fd_list == NULL) {
			perror("malloc");
			exit(EXIT_FAILURE);
		}
		fd_list[0] = STDIN_FILENO;
		*num_files = 1;
	}
	return fd_list;
}

/*
 * function: run_word_freak
 * ------------------------
 *
 * returns : 0
 * type	   : int
 */

int run_word_freak(int argc, char *argv[]) {
	int *fd_list;
	char *all_text;
	int num_files;

	// processes the user input (any form of it) and gets an array of all the file descriptors to be read, and the size of that array
	fd_list = process_input(argc, argv, &num_files);

	// reads and parses the text in the passed files
	Hashmap *hm = read_parse(num_files, fd_list);
	print_frequency(&hm);
	return 0;
}

/*
 * function: main
 * --------------
 * calls run_word_freak
 *
 * returns : 0
 * type	   : int
 */
int main(int argc, char *argv[]) {
	run_word_freak(argc, argv);
	return 0;
}
