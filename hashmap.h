// provides access to all required libraries, macros, and vars
#include "dependencies.h"

/*
 * struct: HM_Entry
 * ----------------
 * models an individual entry in a hashmap
 *
 * next structure allows for linked lists of entries
 *		and bucketing (collision) support: if two different keys
 * 		hash to the same value, appended to the corresponding list
 *
 * key in this context is a word that is read inside of a file
 *
 * value represents the number of occurences of key
 */

typedef struct hm_entry {
	struct hm_entry *next;
	char *key;
	int value;
} HM_Entry;

/*
 * struct: Hashmap
 * ----------------
 * basic implementation of a hashmap, containing a collection of HM_Entry
 *
 * entries is an array of HASH_SIZE with pointers to the entries comprising the map
 */

typedef struct hashmap {
	HM_Entry *entries[HASH_SIZE];
} Hashmap;

// constructors
HM_Entry *init_entry();

Hashmap *init_hashmap();

// calculates the hash value of a provided string using RS Hash
unsigned long get_hash(char *c);

// gets the entry in the Hashmap that corresponds to the passed key, returns a pointer to the entry
HM_Entry *get_entry(Hashmap *hm, char *key);

// enters the key and value into the Hashmap
HM_Entry *set_entry(Hashmap **hm, char *key);
