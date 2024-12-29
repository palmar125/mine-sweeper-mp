CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -fdiagnostics-color=always --std=c99 -g
LDFLAGS = -lncurses
OBJECTS = mine-sweeper-mp.o

mine-sweeper-mp: mine-sweeper-mp.c
	$(CC) $(CFLAGS) -o mine-sweeper-mp mine-sweeper-mp.c $(LDFLAGS)

clean: 
	rm -rf mine-sweeper-mp