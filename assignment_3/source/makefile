.DEFAULT_GOAL:= main

.PHONY: clean
clean:
	rm -f *.o

debug: main.c
	gcc main.c -O0 -g3

taco: main.c taco_krydder taco_saus taco_lefse protein gronnsaker drikke
	gcc -o main.c taco_krydder taco_saus taco_lefse protein gronnsaker drikke

taco_krydder: taco_krydder.c
	gcc -c taco_krydder.c

taco_saus: taco_saus.c
	gcc -c taco_saus.c

taco_lefse: taco_lefse.c
	gcc -c taco_lefse.c

protein: protein.c
	gcc -c protein.c

gronnsaker: gronnsaker.c
	gcc -c gronnsaker.c

drikke: drikke.c
	gcc -c drikke.c

CC:= gcc

CFLAGS:= -O0 -g3
