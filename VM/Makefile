CC=gcc
CFLAGS=-O -Wall -W -pedantic -ansi -std=c99 -ggdb3 -c -O0

vm: vm.o exit.o
	$(CC) -o vm vm.o exit.o

vm.o: vm.c exit.h
	$(CC) $(CFLAGS) vm.c

exit.o: exit.c exit.h
	$(CC) $(CFLAGS) exit.c

clean:
	rm -fr *\.o test
