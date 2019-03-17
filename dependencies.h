
// standard C file IO and Linux Kernel's API IO
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// file control and string manipulations
#include <fcntl.h>
#include <string.h>

// boolean support
#include <stdbool.h>

// relative pathname that allows access to files for testing
#define PATH "./texts/"

// number of characters read from one file at a time
#define BUFF_SIZE 16384

// generous allocation of space for every parsed word
#define MAX_WORD_SIZE 50

// standard output
#define STDOUT 1

// more intuitive descriptor for IO system calls / memory allocation
#define NO_FILE_OPENED -1

#define NO_FILE_READ -1

#define NO_FILE_WRITE -1

#define CLOSE_FAILED -1

// file descriptor array was not initialized successfully
#define FILE_DESC_EMPTY NULL

// malloc or calloc failed
#define MEM_ALLOC_FAIL NULL

// standardized length for each printout of the hashmap
#define FORMATTED_LINE_LENGTH 62

/*
 * size of the hashmap; prime number for a more efficient
 *      hashing algorithm. A larger HASH_SIZE reduces the
 *      number of collisions and bucketing opperations
 */
#define HASH_SIZE 1223
