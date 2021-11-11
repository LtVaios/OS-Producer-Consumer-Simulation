# Paths
INCL = include
SRC = src
MODULES = modules
ODIR = build

# Compiler
CC = gcc

# Compile options
CFLAGS = -Wall -g

# Valgrind flags
VALFLAGS = --leak-check=full --track-origins=yes --trace-children=yes --tool=memcheck --show-leak-kinds=all -s

OBJS = $(MODULES)/client_dir/client.o
OBJS += $(MODULES)/server_dir/server.o
OBJS += $(MODULES)/utils/utilities.o

all: server client
	mkdir -p $(ODIR)
	mv $(OBJS) $(ODIR)

server: clean $(OBJS)
	$(CC) $(CFLAGS) $(MODULES)/server_dir/server.o $(MODULES)/utils/utilities.o -o server -lpthread

client: clean $(OBJS)
	$(CC) $(CFLAGS) $(MODULES)/client_dir/client.o $(MODULES)/utils/utilities.o -o client -lpthread

valgrind: all
	valgrind $(VALFLAGS) ./server

# Delete executable & object files
clean:
	rm -f server
	rm -f client
	rm -rf $(ODIR)