
// struct that models an individual entry in a hashmap
// has values for the entry's name/key, and the value associated with that name/key
// only works when the name is a "string" and the value is an int, as that is what's necessary for this project
typedef struct hm_entry {
	struct hm_entry *next;
	char *key;
	int *value;
} HM_Entry;

typedef struct hashmap {
	HM_Entry **entries;
} Hashmap;

// "constructor" for hashmap struct
struct hashmap *hashmap(struct hashmap *hm);

// calculates the hash value of a provided string using RS Hash
int get_hash(char *c);

// gets the entry in the dictionary that corresponds to the passed value, returns a pointer to the entry
struct hm_entry *get_entry(struct hashmap *hm, char *key);

// enters the key and value into the dictionary, if the key already exists, then replace it's value with the passed value
struct hm_entry *set_entry(struct hashmap *hm, char *key, int *value);
