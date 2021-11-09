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
VALFLAGS = --leak-check=full --track-origins=yes --trace-children=yes -s

OBJS = $(MODULES)/client_dir/client.o
OBJS += $(MODULES)/server_dir/server.o
OBJS += $(MODULES)/utils/utilities.o

all: server client
	mkdir -p $(ODIR)
	mv $(OBJS) $(ODIR)

server: clean $(OBJS)
	$(CC) $(CFLAGS) $(MODULES)/server_dir/server.o -o server

client: clean $(OBJS)
	$(CC) $(CFLAGS) $(MODULES)/client_dir/client.o -o client

valgrind: all
	valgrind $(VALFLAGS) ./server

# Delete executable & object files
clean:
	rm -f server
	rm -f client
	rm -rf $(ODIR)