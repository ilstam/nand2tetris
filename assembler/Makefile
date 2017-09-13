CC=gcc
CFLAGS=-O -Wall -W -pedantic -ansi -std=c99 -ggdb3 -c -O0

assembler: assembler.o symbol_table.o asm_malloc.o exit.o
	$(CC) -o assembler assembler.o symbol_table.o asm_malloc.o exit.o

assembler.o: assembler.c symbol_table.h asm_malloc.h hack_standard.h exit.h
	$(CC) $(CFLAGS) assembler.c

symbol_table.o: symbol_table.c symbol_table.h asm_malloc.h hack_standard.h
	$(CC) $(CFLAGS) symbol_table.c

asm_malloc.o: asm_malloc.c asm_malloc.h exit.h
	$(CC) $(CFLAGS) asm_malloc.c

exit.o: exit.c exit.h
	$(CC) $(CFLAGS) exit.c

clean:
	rm -fr *\.o test
