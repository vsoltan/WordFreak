
#include "hashmap.h"


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
	free(fd_list);
	return word_map;
}

// prints out all the words and their occurences in the processed files
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

// checks if we have to process input through piping from stdin
bool input_piping() {
	printf("input_piping()");
	// initializes input by adding the fd for stdin to a fd_set
	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(STDIN_FILENO, &readfds);

	// we only want to see if we are already able to read from stdin
	// so we set it to timeout after 0 seconds
	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;

	//gets the number of fds that are ready to be read from
	int select_returnval = select(1, &readfds, NULL, NULL, &timeout);

	// if there are files set to be read, and stdin is one of those files, we are ready for piping
	return select_returnval > 0 && FD_ISSET(STDIN_FILENO, &readfds) != 0;
}

// take the cmd parameters from main()
// returns an array of file descriptors
int *process_input(int argc, char *argv[], int *num_files) {
	//debug
	printf("process_input()\n");

	// array containing all read file descriptors
	// TODO free return_val in main - done
	int *fd_list;

	 // ENVIRONMENTAL VARIABLE
	 if (getenv("WORD_FREAK")) {
		 printf("it exists!\n"); // TODO remove printf
	 } else if(input_piping()) {
		 // PIPING
		 printf("piping\n"); // TODO remove printf
		 fd_list = malloc(sizeof(int));
		 if (fd_list == NULL) {
			 perror("malloc");
			 exit(EXIT_FAILURE);
		 }
		 // gets the fd of standard in and puts it in the list
		 fd_list[0] = STDIN_FILENO;
		 *num_files = 1;
	 } else {
		 // CMD ARGUMENTS (Default case)
		 printf("cmd arguments\n"); // TODO remove printf
		 fd_list = malloc(sizeof(int) * (argc - 1));
		 // checking that malloc worked
		 if (fd_list == NULL) {
			 perror("malloc");
			 exit(EXIT_FAILURE);
		 }

		 // gets all the fd from the files in the cmd line args and adds them to the list
		 fd_list = open_files(argc - 1, argv + 1);
		 *num_files = argc - 1;
		 // null checking
		 if(fd_list == NULL) {
			 exit(EXIT_FAILURE);
		 }	 
	}
	return fd_list;
}

int run_word_freak(int argc, char *argv[]) {
	int *fd_list;
	char *all_text;
	int num_files;

	// processes the user input (any form of it) and gets an array of all the file descriptors to be read, and the size of that array
	fd_list = process_input(argc, argv, &num_files);

	// reads and parses the text in the passed files
	Hashmap *hm = read_parse(num_files, fd_list);
	print_word_occ(hm);
	return 0;
}

int main(int argc, char *argv[]) {
	run_word_freak(argc, argv);
}
