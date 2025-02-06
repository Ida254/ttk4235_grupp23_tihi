CC:= gcc
CFLAGS:= -O0 -g3
OBJ:= taco_krydder.o taco_saus.o taco_lefse.o protein.o grønnsaker.o drikke.o main.o

.DEFAULT_GOAL:= taco

clean:
	rm -f *.o taco

taco: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)
