main.o: main.c constants.h taco.c
	gcc -c main.c -o main.o

.DEFAULT_GOAL:= taco

.PHONY: clean
clean:
	rm -f *.o

debug: main.c
	gcc main.c -0O -g3

taco: taco.c
	gcc -c taco.c

CC:= gcc

CFLAGS:= -0O -g3
