
#include "hashmap.h"
#include <string.h>

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

	for(char *c = key; c* != '\0'; ++c) {
		hash = (hash * a) + (*c);
		a *= b;
	}

	return hash % HASH_SIZE;
}

char *copy_string(char *c1) {
	// need to free later
	char *c2 = malloc(strlen(c1) * sizeof(char) + 1);
	if(c2 == NULL) {
		printf("could not malloc c2\n");
		exit(EXIT_FAILURE);
	}

	strcpy(c2, c1);

	return c2;
}


// TODO: implement dictionary methods

Dictionary_Entry *get_entry(char *key) {
	for(HM_Entry *entry = hashmap(get_hash(key)); entry != NULL; entry = entry->next) {
		if(strcmp(key, entry->name) == 0) {
			return entry;
		}
	}

	printf("entry does not exist\n");
	return NULL;
}

Dictionary_Entry *set_entry(char *key, int *value) {
	HM_Entry *entry = get_entry(key);
	int hash = get_hash(key);

	// entry not found
	if(entry == NULL) {
		// add entry to hashmap
		entry = malloc(sizeof(HM_Entry));

		// could not malloc
		if(entry == NULL || entry->next == NULL || entry->key == NULL || entry->value == NULL) {
			printf("could not sucessfully malloc\n");
			exit(EXIT_FAILURE);
		}

		hashmap[hash] = entry;

	} else {
		// if entry already exists free the value
		free(entry->value);
	}

	char *value_copy = malloc(strlen(c1) * sizeof(char) + 1);
	if(value_copy == NULL) {
		printf("could not malloc value_copy\n");
		exit(EXIT_FAILURE);
	}

	strcpy(value_copy, value);

	entry->value = value_copy;

	return entry;
}
