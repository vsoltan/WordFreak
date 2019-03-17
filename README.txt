Word Freak

Contributors:
 - Brendon Ky
 - Deniz Guler
 - Valeriy Soltan

Files:
 - dependencies.h
    - lists all of the headers that are used across the project
 - hashmap.c/hashmap.h
    - implementation of a hashmap data structure in c
 - main.c
    - handles the execution of the project
 - readparse.c/readparse.h
    - handles the reading and parsing of files
 - Makefile
    - allows the use of "$ make" in bash
 - ./texts/
    - directory for text files that the program could parse

Summary:
    The aim of this project is to be able to read text files specified by the
user, and count the number of times each unique word occurs in all of the files.

    The user can specify which text files should be parsed in 3
different ways:
    1) through command line arguments
        $ ./wordfreak a.txt b.txt

    2) through bash piping to stdin
        $ cat texts/a.txt texts/b.txt | ./wordfreak
        $ echo "this is a sentence" | ./wordfreak
       ---
        $ ./wordfreak
        $ this is also a sentence

    3) through an environmental variable called WORD_FREAK
        $ WORD_FREAK=a.txt ./wordfreak

    After handling the user input, we read the specified files in blocks of
16384 (which is 2^14) characters. After a block of characters is read onto a
buffer, it is parsed so that continous sections of alphanumeric characters,
separated by whitespace characters are considered to be a word. We do not
differentiate words by their capitalization, so that "WORD", "word", "Word", and
and "wORd", are all treated the same.

    After a word is parsed, it is put into a hashmap struct that we implemented
with the exclusion of the hashing algorithm. The hashing algorithm we
implemented is djb2 by Dan Bernstein.

    After the words in every file are parsed, we display the results
on the console, telling the user how may times each word occured.
