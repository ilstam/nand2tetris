CC=gcc
CFLAGS=-O -Wall -W -pedantic -ansi -std=c99 -ggdb3 -c

assembler: assembler.o symbol_table.o utils.o
	$(CC) -o assembler assembler.o symbol_table.o utils.o

assembler.o: assembler.c symbol_table.h utils.h
	$(CC) $(CFLAGS) assembler.c

symbol_table.o: symbol_table.c symbol_table.h utils.h
	$(CC) $(CFLAGS) symbol_table.c

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) utils.c

clean:
	rm -fr *\.o test
