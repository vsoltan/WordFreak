
#include "hashmap.h"

/*
 * function: init_hashmap
 * ----------------------
 * initializes a hashmap and sets its entries to NULL
 *
 * returns : the pointer to the new hashmap
 * type	   : (Hashmap *)
 */

Hashmap *init_hashmap() {
	// dynamically allocates space for the hashmap
	Hashmap *map = malloc(sizeof(Hashmap));
	// checks that malloc was completed successfully
	if(map == MEM_ALLOC_FAIL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	// initialize the list of entries to NULL
	for (int i  = 0; i < HASH_SIZE; ++i) {
		map->entries[i] = NULL;
	}
	return map;
}

// frees the hashmap and all its entries by calling free_entry
void free_hashmap(Hashmap **map) {
	// frees all the entries in the map
	for(int i = 0; i < HASH_SIZE; ++i) {
		free_entry(&((*map)->entries[i]));
	}
	// frees the entire structure once all fields are returned to heap
	free(*map);
	// avoids dangling pointer
	*map = NULL;
}

/*
 * function: init_entry
 * --------------------
 * initializes an HM_Entry and mallocs space for its key
 *
 * returns : the pointer the new entry
 * type	   : (HM_Entry *)
 */

HM_Entry *init_entry() {
	HM_Entry *new_entry = (HM_Entry *) malloc(sizeof(HM_Entry));
	// checks that malloc was completed successfully
	if (new_entry == MEM_ALLOC_FAIL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	// allocates space for the key field
	new_entry->key = (char *) malloc(MAX_WORD_SIZE);
	// checks that malloc was completed successfully
	if (new_entry->key == MEM_ALLOC_FAIL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	return new_entry;
}

// frees an entry and its linked list recursively
void free_entry(HM_Entry **entry) {
	if (*entry == NULL) {
		return;
	}
	// free the last entry in the linked list first
	free_entry(&(*entry)->next);
	// free the key field before freeing the entire struct
	free((*entry)->key);
	free(*entry);
	// avoid dangling pointer
	*entry = NULL;
}

/*
 * function: get_hash
 * ------------------
 * gets the hash for the given key
 *		utilizes the hash algorithm djb2
 *      implementation: http://www.cse.yorku.ca/~oz/hash.html
 *
 * had to use unsigned keyword because value for hash was exceeding limit
 *
 * returns : the hash of the given key
 * type	   : unsigned long
 */

unsigned long get_hash(char *key) {
	unsigned long hash = 5381;
	int c;
	char *dummy = key;
	while (c = *dummy++) {
		hash = ((hash << 5) + hash) + c;
	}
	return hash % HASH_SIZE;
}

/*
 * function: get_entry
 * -------------------
 * gets the entry in the hashmap that corresponds to the passed key
 *
 * checks if the hashed index contains a value, if not, proceeds
 *      to check the entries in the linked list, as to retrieve
 *      colliding entries
 *
 * returns : the pointer to the entry
 * type	   : (HM_Entry *)
 */

HM_Entry *get_entry(Hashmap *map, char *key) {
	// handles collision of keys in the hashmap by iterating through the linked list
	for(HM_Entry *entry = map->entries[get_hash(key)]; entry != NULL; entry = entry->next) {
		if (strcmp(key, entry->key) == 0) {
			return entry;
		}
	}
	// return NULL if the entry is non-existant
	return NULL;
}

/*
 * function: increment_entry
 * -------------------------
 * attempts to increment the entry with the given key
 *      if the key doesn't exist, then it creates a new entry
 *      if the key already exists, then it increments the current value of that entry
 *
 * returns : the incremented entry
 * type	   : (HM_Entry *)
 */

HM_Entry *increment_entry(Hashmap **map, char *key) {
	// NULL if empty
	HM_Entry *entry = get_entry(*map, key);
	int hash = get_hash(key);
	// if the entry is not found
	if (entry == NULL) {
		// creates a new entry
		HM_Entry *new_entry = init_entry();
		// assigns the passed key to the new entry
		strcpy(new_entry->key, key);
		// if entry is initialized for the first time, counter set to 1
		new_entry->value = 1;
		// front-end insertion of the entry into the linked list to handle collisions
		HM_Entry *temp = (*map)->entries[hash];
		new_entry->next = temp;
		(*map)->entries[hash] = new_entry;
		// set the returned entry to the new one
		entry = new_entry;
	} else {
		// if entry already exists, then increment its value
		entry->value = entry->value + 1;
	}
	return entry;
}
