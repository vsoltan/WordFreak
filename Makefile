
SRC  = main.c hashmap.c readparse.c
OBJ  = main.o hashmap.o readparse.o
PROG = wordfreak

$(PROG): $(OBJ)
	gcc $(OBJ) -o $(PROG)

$(OBJ): $(SRC)

# dependencies
main.o: readparse.h dependencies.h
hashmap.o: hashmap.h dependencies.h
readparse.o: readparse.h dependencies.h
