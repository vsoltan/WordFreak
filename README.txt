Word Freak

Contributors:
 - Brendon Ky
 - Deniz Guler
 - Valeriy Soltan

Files:
 - dependencies.h
    - lists all of the headers, macros, and constants that are used across the project
 - hashmap.c / hashmap.h
    - implementation of a hashmap data structure in C
 - main.c
    - handles the execution of the project
 - readparse.c / readparse.h
    - handles the reading and parsing of files
 - Makefile
    - allows the use of "$ make" in bash, automatically re-linking object files
 - ./texts/
    - directory for text files that the program can parse

Summary:

The aim of this project is to be able to read text files specified by the
    user, and count the number of times each unique word occurs in all of the files.

The user can specify which text files should be parsed in 3
    different ways (command handling is done in the method process_input()):

    1) through command line arguments
        $ ./wordfreak a.txt b.txt c.txt

    2) through bash piping to stdin
        $ cat texts/a.txt texts/b.txt | ./wordfreak
        $ echo "this is a sentence" | ./wordfreak
        ---
        $ ./wordfreak
        $ this is also a sentence

    3) through an environmental variable called WORD_FREAK
        $ WORD_FREAK=a.txt ./wordfreak

After handling the user input, we read the specified files in blocks of
    16384 (2^14) characters. After a block of characters is read onto a
    buffer, it is parsed so that continuous sections of alphanumeric characters,
    separated by whitespace characters are considered to be a word. We do not
    differentiate words by their capitalization, so that "WORD", "word", "Word", and
    and "wORd", are all treated the same. The buffer is then reset and, continues
    to process the file until the end is reached.

After a word is parsed, it is put into a hashmap struct that we implemented.
    (excluding the hashing algorithm). The hashing algorithm we implemented is
    djb2 by Dan Bernstein. A hash value is generated for every word that is
    encountered in the text files. The entry containing each word, along with
    its occurrences, can be retrieved by passing the word to the hashmap through
    our method get_entry(), which searches the hashed index as well as its
    connections to find the specified word. Because the size of the hashmap is
    predefined and finite, there are going to be certain words that hash to the same value,
    resulting in collisions. This is handled by making each entry a node in a
    linked list of entries; therefore, when a collision inevitably occurs, the word
    is just added to the corresponding linked list using front insertion.

Once all the files are processed (the words and their occurrences occupying the hashmap),
    the method print_frequency() iterates through the hashmap and its buckets, formatting
    a combination of the word | frequency, and printing them out in the console.

Requirements:
1)  Makefile is included, handling the interdependence of the files in our implementation
2)  Source and headers are supplied as specified (see above for rundown of the files and their functions)
3)  Modularity in design, comments throughout the implementation, and proper indentation
4)  README.txt supplied
5)  Program handles a variety of inputs (as shown above)
6)  Environmental variable support is included
7)  Hashmap and its entries (HM_Entry) are dynamically allocated (and freed) to accommodate different file sizes
8)  Structures and used to represent hashmap and its entries
9)  Pointers are passed to functions, modified, and used extensively throughout the implementation
10) Linux kernel API IO is used
11) sprintf is used for formatting (main.c -> print_frequency)
12) Error checking used to determine the success of system calls and malloc/calloc calls
