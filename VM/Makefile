CC=gcc
CFLAGS=-O -Wall -W -pedantic -ansi -std=gnu99 -ggdb3 -c -O0 -fsanitize=address
LDFLAGS=-fsanitize=address

vm: vm.o utils.o exit.o
	$(CC) -o vm vm.o utils.o exit.o $(LDFLAGS)

vm.o: vm.c utils.h mapper.h exit.h
	$(CC) $(CFLAGS) vm.c utils.c

exit.o: exit.c exit.h
	$(CC) $(CFLAGS) exit.c

clean:
	rm -fr *\.o test
