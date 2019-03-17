
# include "hashmap.h"

// accepts pointer to a hashmap and initializes the array
// returns the pointer to the hashmap passed

Hashmap *init_hashmap() {
	Hashmap *map = malloc(sizeof(Hashmap));
	if(map == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	// initialize the entries in the list of entries to NULL
	for (int i  = 0; i < HASH_SIZE; ++i) {
		map->entries[i] = NULL;
	}
	return map;
}

void free_hashmap(Hashmap **map) {
	for(int i = 0; i < HASH_SIZE; ++i) {
		free_entry(&((*map)->entries[i]));
	}
	// frees the entire structure once all fields are returned to heap
	free(*map);
	*map = NULL;
}

HM_Entry *init_entry() {
	HM_Entry *new_entry = (HM_Entry *) malloc(sizeof(HM_Entry));
	// could not malloc
	if (new_entry == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	new_entry->key = (char *) malloc(MAX_WORD_SIZE);
	// could not malloc
	if (new_entry->key == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	return new_entry;
}

void free_entry(HM_Entry **entry) {
	// puts the freeing of the last entry at the top of the stack function linkage
	if (*entry != NULL) {
		free_entry(&(*entry)->next);
	} else {
		// frees the first entry in the list
		free(*entry);
		// dangling pointer
		*entry = NULL;
	}
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
	// printf("inside get entry\n");
	// handles collision of keys in the hashmap TODO DOESN'T HANDLE COLLISIONS YET
	HM_Entry *entry;
	for(entry = hm->entries[get_hash(key)]; entry != NULL; entry = entry->next) {
		// printf("before check\n");
		if (strcmp(key, entry->key) == 0) {
			// printf("inside if statement\n");
			return entry;
		}
	}
	// printf("entry does not exist [not an error]\n");
	return NULL;
}

// enters the key and value into the dictionary,
// if the key doesn't exist, then create a new entry
// if the key already exists, then replace it's current value with the passed value
HM_Entry *increment_entry(Hashmap **hm, char *key) {
	// NULL if empty
	HM_Entry *entry = get_entry(*hm, key);
	int hash = get_hash(key);
	// if the entry not found
	if (entry == NULL) {
		// then create a new entry
		HM_Entry *new_entry = init_entry();
		strcpy(new_entry->key, key);

		// if entry is initialized for the first time, counter set to 1
		new_entry->value = 1;
		// this line is problematic:
		HM_Entry *temp = (*hm)->entries[hash];
		new_entry->next = temp;
		(*hm)->entries[hash] = new_entry;
		// printf("entries[hash : %i] = %s\n", hash, (*hm)->entries[hash]->key);
		entry = new_entry;
	} else {
		// if entry already exists, then free the old value so it can be updated
		entry->value = entry->value + 1;
	}
	return entry;
}
