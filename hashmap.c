
#include "hashmap.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// increase HASH_SIZE to reduce collision in the dicttionary
// should be a prime number
#define HASH_SIZE 97

// static implementation of the dictionary
static HM_Entry *hashmap[HASH_SIZE];

// implements a basic hash algorithm based on the value of the pointer passed
// Uses RS Hash https://stackoverflow.com/questions/6136964/rs-hashing-program
int get_hash(char *key) {
	int b = 378551;
	int a = 63689;

	int hash = 0;

	for(char *c = key; *c != '\0'; ++c) {
		hash = (hash * a) + (*c);
		a *= b;
	}

	return hash % HASH_SIZE;
}

// gets the entry in the dictionary that corresponds to the passed value,
// returns a pointer to the that entry
HM_Entry *get_entry(char *key) {
	for(HM_Entry *entry = hashmap[get_hash(key)]; entry != NULL; entry = entry->next) {
		if(strcmp(key, entry->key) == 0) {
			return entry;
		}
	}

	printf("entry does not exist\n");
	return NULL;
}

// enters the key and value into the dictionary,
// if the key doesn't exist, then create a new entry
// if the key already exists, then replace it's current value with the passed value
HM_Entry *set_entry(char *key, int *value) {
	HM_Entry *entry = get_entry(key);
	int hash = get_hash(key);

	// if the entry not found
	if(entry == NULL) {
		// then create a new entry
		entry = malloc(sizeof(HM_Entry));

		// could not malloc
		if(entry == NULL || entry->next == NULL || entry->key == NULL || entry->value == NULL) {
			printf("could not sucessfully malloc\n");
			exit(EXIT_FAILURE);
		}

		hashmap[hash] = entry;

	} else {
		// if entry already exists, then free the old value so it can be updated
		free(entry->value);
	}

	// duplicates the value 
	// don't want the entry to have a pointer to what was passed to maintain encapsulation
	int *value_copy = malloc(sizeof(int));
	if(value_copy == NULL) {
		printf("could not malloc value_copy\n");
		exit(EXIT_FAILURE);
	}

	*value_copy = *value;

	// assigns new/updated value
	entry->value = value_copy;

	return entry;
}
