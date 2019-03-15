
#define HASH_SIZE 101


// struct that models an individual entry in a hashmap
// has values for the entry's name/key, and the value associated with that name/key
// only works when the name is a "string" and the value is an int, as that is what's necessary for this project
typedef struct hm_entry {
	struct hm_entry *next;
	char *key;
	int value;
} HM_Entry;

typedef struct hashmap {
	HM_Entry *entries[HASH_SIZE];
} Hashmap;

// constructors
HM_Entry *init_entry();

Hashmap *init_hashmap();

// calculates the hash value of a provided string using RS Hash
unsigned long get_hash(char *c);

// gets the entry in the dictionary that corresponds to the passed value, returns a pointer to the entry
HM_Entry *get_entry(Hashmap *hm, char *key);

// enters the key and value into the dictionary, if the key already exists, then replace it's value with the passed value
HM_Entry *set_entry(Hashmap **hm, char *key);
