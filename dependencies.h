
// standard C file IO and Linux Kernel's API IO
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// wrapper functions for system calls
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>

// file and string manipulations
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

// more intuitive descriptor for when read() fails
#define NO_FILE_OPENED -1

// standardized length for each printout of the hashmap
#define FORMATTED_LINE_LENGTH 62

/*
 * size of the hashmap; prime number for a more efficient
 *      hashing algorithm. A larger HASH_SIZE reduces the
 *      number of collisions and bucketing opperations
 */
#define HASH_SIZE 1223
