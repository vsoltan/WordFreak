
#include "hashmap.h"

// checks if the passed char is not a whitespace (space, tab, enter, etc)
#define IS_WHITESPACE(c) c == ' ' || c == '\n' || c == '\t' || c == '\v' || c == '\f' || c == '\r'

// checks if the passed char is alphanumeric or a '\''
#define IS_ALPHANUMERIC(c) 'a' <= c && c <= 'z' || 'A' <= c && c <= 'Z' || c == '\'' || '0' <= c && c <= '9'

// converts uppercase characters to lowercase
static char to_lowercase(char c);

// reads the passed files, constructs words based on specifications, and adds them to hashmap
Hashmap *read_parse(int num_files, int *fd_list);
