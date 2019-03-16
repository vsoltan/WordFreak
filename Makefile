
SRC  = main.c hashmap.c
OBJ  = main.o hashmap.o
PROG = wordfreak

$(PROG): $(OBJ)
	gcc $(OBJ) -o $(PROG)

$(OBJ): $(SRC)
main.o: dependencies.h
main.o: hashmap.h
hashmap.o: hashmap.h
