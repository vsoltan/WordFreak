
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "hashmap.h"

// increase HASH_SIZE to reduce collision in the dictionary
// should be a prime number

// accepts pointer to a hashmap and initializes the array
// returns the pointer to the hashmap passed
Hashmap *init_hashmap() {
	Hashmap *hm = malloc(sizeof(Hashmap));
	hm->entries = calloc(HASH_SIZE, sizeof(HM_Entry *));
	if(hm == NULL || hm->entries == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	return hm;
}

// uses the hash algorithm djb2
// implementation: http://www.cse.yorku.ca/~oz/hash.html
unsigned long get_hash(char *key) {
	unsigned long hash = 5381, hash_size = 97;
	int c;
	char *dummy = key;
	while (c = *dummy++) {
		hash = ((hash << 5) + hash) + c;
	}
	return hash % HASH_SIZE;
}

// gets the entry in the dictionary that corresponds to the passed value,
// returns a pointer to the that entry
HM_Entry *get_entry(Hashmap *hm, char *key) {
	// handles collision of keys in the hashmap

	// printf("%p\n", hm->entries[get_hash(key)]);
	for(HM_Entry *entry = hm->entries[get_hash(key)]; entry != NULL; entry = entry->next) {
		// printf("for loop iteration\n");
		// because short circuit evaluation does not exist
		if (entry != NULL) {
			if (strcmp(key , entry->key) == 0) {
				// printf("entered the if statement\n");
				return entry;
			}
		}
	}
	printf("entry does not exist [not an error]\n");
	return NULL;
}

// enters the key and value into the dictionary,
// if the key doesn't exist, then create a new entry
// if the key already exists, then replace it's current value with the passed value
HM_Entry *set_entry(Hashmap *hm, char *key) {

	// NULL if empty
	HM_Entry *entry = get_entry(hm, key);
	if (entry == NULL) {
		printf("there is nothing there before insertion\n");
	}
	int hash = get_hash(key);
	printf("%i,\n", hash);
	// if the entry not found
	if (entry == NULL) {
		// then create a new entry
		entry = (HM_Entry *) malloc(sizeof(HM_Entry));
		// could not malloc
		if (entry == NULL) {
			perror("malloc");
			exit(EXIT_FAILURE);
		}

		entry->key = key;
		// if entry is initialized for the first time, counter set to 1
		entry->value = 1;

		hm->entries[hash] = entry;
		printf("insertion complete?\n");
		printf("entries[hash] = %s\n", hm->entries[hash]->key);
	} else {
		// if entry already exists, then free the old value so it can be updated
		entry->value = entry->value + 1;
	}

	return entry;
}
