
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "hashmap.h"

// increase HASH_SIZE to reduce collision in the dictionary
// should be a prime number
#define HASH_SIZE 97

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

// implements a basic hash algorithm based on the value of the pointer passed
// Uses RS Hash algorithm found in https://stackoverflow.com/questions/6136964/rs-hashing-program
// static unsigned int get_hash(char *key) {
// 	unsigned int a = 63689, b = 378551;
// 	unsigned int hash = 0;
//
// 	for(char *c = key; *c != '\0'; ++c) {
// 		hash = (hash * a) + (*c);
// 		a *= b;
// 	}
// 	printf("Hash value: %u", hash % HASH_SIZE);
// 	return hash % HASH_SIZE;
// }

// uses the hash algorithm djb2
// implementation: http://www.cse.yorku.ca/~oz/hash.html
unsigned long get_hash(char *key) {
	unsigned long hash = 5381, hash_size = 97;
	int c;

	while (c = *key++) {
		// printf("%c ", (char) c);
		hash = ((hash << 5) + hash) + c;
		// printf("\nhash : %lu\n", hash % HASH_SIZE);
	}
	return hash % HASH_SIZE;
}

// gets the entry in the dictionary that corresponds to the passed value,
// returns a pointer to the that entry
HM_Entry *get_entry(Hashmap *hm, char *key) {
	// handles collision of keys in the hashmap

	// printf("%p\n", hm->entries[get_hash(key)]);
	for(HM_Entry *entry = hm->entries[get_hash(key)]; entry != NULL; entry = entry->next) {
		printf("for loop iteration\n");
		// because short circuit evaluation does not exist
		if (entry != NULL) {
			printf("entry is not null\n");
			printf("key is : %s\n", key);
			printf("entry is : %s\n", entry->key);
			//if (strcmp(key, entry->key) == 0) {
			// TODO: entry->key is not malloced?
			if (strcmp(key , entry->key) == 0) {
				printf("entered the if statement\n");
				return entry;
			}
		}
		printf("ended for loop\n");
	}
	// debugging
	printf("entry does not exist [not an error]\n");
	return NULL;
}

// enters the key and value into the dictionary,
// if the key doesn't exist, then create a new entry
// if the key already exists, then replace it's current value with the passed value
HM_Entry *set_entry(Hashmap *hm, char *key, int *value) {
	HM_Entry *entry = get_entry(hm, key);

	int hash = get_hash(key);
	// if the entry not found
	if (entry == NULL) {
		// then create a new entry
		entry = (HM_Entry *) malloc(sizeof(HM_Entry));
		entry->key = key;

		// could not malloc
		if (entry == NULL || entry->key == NULL) {
			perror("malloc");
			exit(EXIT_FAILURE);
		}

		(hm->entries)[hash] = entry;

	} else {
		// if entry already exists, then free the old value so it can be updated
		free(entry->value);
	}

	// duplicates the value
	// don't want the entry to have a pointer to what was passed to maintain encapsulation
	int *value_copy = malloc(sizeof(int));
	if (value_copy == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	*value_copy = *value;

	// assigns new/updated value
	entry->value = value_copy;

	return entry;
}
