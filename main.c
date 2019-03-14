
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define PATH "./texts/"
#define BUFF_SIZE 256
#define NO_DATA_READ 0
#define NO_FILE_OPENED -1

// takes an array file names, and the number of files and opens those files in the directory specified by the constant PATH
// returns an array containing the file descriptors for every file specified
int *open_files(int num, char **file_names) {
	// list of the file descriptors
	int *fd_list = calloc(num, sizeof(int));
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

void read_files(int numfiles, int *fdlist, char **all_text) {
	char buffer[BUFF_SIZE] = "\0";
	printf("kill me\n");
	char *text_ptr = (char *) malloc(BUFF_SIZE);
	printf("kill me2\n");
	int num_read = 0;
	// iterates the loop until all of the files are read
	for (int i = 0, j = 0; j < numfiles; ++i) {
		num_read = read(fdlist[j], buffer, BUFF_SIZE);
		printf("%i\n", num_read);
		strcat(text_ptr, buffer);
		buffer[0] = '\0';
		// perror("read");
		// if EOF is reached
		// if (num_read == NO_DATA_READ) {
		//
		// }
		if (num_read < BUFF_SIZE) {
			text_ptr[BUFF_SIZE * i + num_read] = '\0';
			printf("tiger\n");
			++j;
		}

		if (j < numfiles) {
			printf("salmoo0n\n");
			char *tmp_ptr = (char *) realloc(text_ptr, (i + 2) * BUFF_SIZE);
			printf("yo shawty how you do mayne\n");
			free(text_ptr);
			printf("suh cuh\n");
			text_ptr = malloc((i + 2) * BUFF_SIZE);
			printf("pepo happy\n");
			strcpy(text_ptr, tmp_ptr);
			printf("work dammit\n");
			free(tmp_ptr);
			printf("tim $$$ help\n");
			// memcpy(text_ptr, tmp_ptr);
			// perror("realloc");
			printf("salmoo1n\n");
		}
	}
	*all_text = text_ptr;
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
	// char all_text[][BUFF_SIZE];
	char **all_text;

	printf("test\n");
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
		printf("oi\n");
		// read(fd_list[0], all_text, BUFF_SIZE);
		read_files(argc - 1, fd_list, all_text);
	}
	//
	// for (int i = 0; i < BUFF_SIZE; ++i) {
	// printf("%s\n", all_text);

	return 0;
}
