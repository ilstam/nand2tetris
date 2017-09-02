CC=gcc
CFLAGS=-O -Wall -W -pedantic -ansi -std=c99 -ggdb3 -c

assembler: assembler.o symbol_table.o utils.o exit.o
	$(CC) -o assembler assembler.o symbol_table.o utils.o exit.o

assembler.o: assembler.c symbol_table.h utils.h hack_standard.h exit.h
	$(CC) $(CFLAGS) assembler.c

symbol_table.o: symbol_table.c symbol_table.h utils.h hack_standard.h
	$(CC) $(CFLAGS) symbol_table.c

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) utils.c

exit.o: exit.c exit.h
	$(CC) $(CFLAGS) exit.c

clean:
	rm -fr *\.o test
