
SRC = main.c hashmap.c
OBJ = main.o hashmap.o
PROG = wordfreak
DEPS = hashmap.h

%.o: %.c $(DEPS)
	gcc -c -o $@ $<

$(PROG): $(OBJ)
	gcc -o $(PROG) $(OBJ)
#hashmap.o : $(DEPS)

#-include $(DEPS)
