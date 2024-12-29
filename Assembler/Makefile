# Compiler and Flags
CC = gcc
CFLAGS = -Wall -std=c99 -g

# Target for assembler
all: assembler

assembler: assembler.o assembler_main.o
	$(CC) $(CFLAGS) -o assembler assembler.o assembler_main.o

assembler.o: assembler.c assembler.h
	$(CC) $(CFLAGS) -c assembler.c -o assembler.o

assembler_main.o: assembler_main.c assembler.h
	$(CC) $(CFLAGS) -c assembler_main.c -o assembler_main.o

# Clean target
clean:
	rm -f assembler assembler.o assembler_main.o

