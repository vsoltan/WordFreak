#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define PATH "./texts/"

int *open_files(int num, char **file_names) {

	printf("test\n");
	int *fdlist = calloc(num, sizeof(int));
	for (int i = 0; i < num; ++i) {
		char file_path[256] = PATH;
		strcat(file_path, file_names[i]);
		if ((fdlist[i] = open(file_path, O_RDONLY)) == -1) {
			return NULL;
		}
	}
	return fdlist;
}

// char *read_files(int *fdlist) {
//  char cur;
//  read(fdlist[0], &cur, 1);
//  return 0;
// }


/*
 * Basic project functionality outline:
 *  - Get user input
 *     - Command line arguments
 *     - Standard input
 *     - Environment variable WORD_FREAK
 *  - Sanitize input
 *  - Iterate over input
 *  - Count each occurence of each word
 *     - Manage each occurence in a dictionary?
 */
int main(int argc, char *argv[]) {
	int *foo;
	char bar[6];
	printf("test\n");
	switch(argc) {
	// too few inputs
	case 1:
		printf("fail\n");
		exit(EXIT_FAILURE);
	default:
		printf("test %i\n", argc - 1);
		if ((foo = open_files(argc - 1, argv + 1)) == NULL) {
			printf("fail\n");
			exit(EXIT_FAILURE);
		}
		read(*foo, bar, 6);
		// read_files(foo, &bar);
		// read(open("./texts/beemovie.txt", O_RDONLY), bar, 6);
		// run
	}
	// for (int i = 0; i < argc-1; ++i) {
	//  printf("%i \n", foo + i);
	// }
	printf("text: %s \n", bar);
	// Sanitize inputs

	return 0;
}
