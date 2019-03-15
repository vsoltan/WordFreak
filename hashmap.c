
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
	if(hm == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	return hm;
}

HM_Entry *init_entry() {
	HM_Entry *new_entry = (HM_Entry *) malloc(sizeof(HM_Entry));
	new_entry->key = (char *) malloc(50); //MAX_WORD_SIZE
}

// uses the hash algorithm djb2
// implementation: http://www.cse.yorku.ca/~oz/hash.html
unsigned long get_hash(char *key) {
	unsigned long hash = 5381;
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
	// handles collision of keys in the hashmap TODO DOESN'T HANDLE COLLISIONS YET
	for(HM_Entry *entry = hm->entries[get_hash(key)]; entry != NULL; entry = entry->next) {
		// because short circuit evaluation does not exist
		if (entry != NULL) {
			if (strcmp(key , entry->key) == 0) {
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
HM_Entry *set_entry(Hashmap **hm, char *key) {
	// NULL if empty
	HM_Entry *entry = get_entry(*hm, key);
	int hash = get_hash(key);
	// if the entry not found
	if (entry == NULL) {
		// then create a new entry
		HM_Entry *new_entry = init_entry();
		// could not malloc
		if (new_entry == NULL) {
			perror("malloc");
			exit(EXIT_FAILURE);
		}

		printf("space is not properly allocated!\n");
		strcpy(new_entry->key, key);
		// new_entry->key = key;
		printf("%s\n", new_entry->key);
		// if entry is initialized for the first time, counter set to 1
		new_entry->value = 1;

		(*hm)->entries[hash] = new_entry;
		printf("entries[hash : %i] = %s\n", hash, (*hm)->entries[hash]->key);
		entry = new_entry;
	} else {
		// if entry already exists, then free the old value so it can be updated
		entry->value = entry->value + 1;
	}

	return entry;
}
