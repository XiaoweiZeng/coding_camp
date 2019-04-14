CC=gcc
SRC=$(wildcard *.c)
OBJ_FILE=$(wildcard *.o)
OBJ=action
LIBRARY=m


$(OBJ): $(SRC)
	@$(CC) $(SRC) -o $(OBJ) -l$(LIBRARY)

clean:
	@rm -f $(OBJ_FILE)
	@rm -f $(OBJ)

