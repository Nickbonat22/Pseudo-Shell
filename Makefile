CC = gcc
CFLAGS = -g -Wall

default: pseudo-shell

pseudo-shell: main.c command.c
	gcc -o pseudo-shell main.c

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

command.o: command.c command.h
	$(CC) $(CFLAGS) -c command.c

clean: 
	$(RM) pseudo-shell *.o *~

