
SRC = Main.c hashmap.c
OBJ = Main.o hashmap.o

PROG = wordfreak
DEPS = hashmap.h

$(PROG): $(OBJ)
	gcc $(OBJ) -o $(PROG)

$(OBJ): $(SRC)

-include $(DEPS)