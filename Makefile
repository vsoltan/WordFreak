
SRC  = main.c hashmap.c
OBJ  = main.o hashmap.o
PROG = wordfreak
DEPS = hashmap.h

$(PROG): $(OBJ)
	gcc $(OBJ) -o $(PROG)

$(OBJ): $(SRC)
main.o: hashmap.h
hashmap.o: hashmap.h
