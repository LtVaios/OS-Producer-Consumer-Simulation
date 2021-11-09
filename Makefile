# Paths
INCL = include
SRC = src
MODULES = src
ODIR = build

# Compiler
CC = gcc

# Compile options
CFLAGS = -Wall -g -I$(INCL) -I$(MODULES)/main
# Valgrind flags
VALFLAGS = --leak-check=full --track-origins=yes --trace-children=yes -s

OBJS = $(MODULES)/main.o

all: main
	mkdir -p $(ODIR)
	mv $(OBJS) $(ODIR)

main: clean $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) main

valgrind: all
	valgrind $(VALFLAGS) ./main

# Delete executable & object files
clean:
	rm -f main
	rm -rf $(ODIR)