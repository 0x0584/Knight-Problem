# Anas Rchid - fall 2016
# Knight's Tour Makefile
# License: LGPL (v3)

CC = gcc
CFLAGS = -Wall -Werror -ggdb -std=c99
OBJ = kt.o
SRC = kt.c
LDFLAGS = -lpopt
ARGS = -d -R -A -r 12 -c 16 -w 25 -h 25
EXEC = x
all:
	$(CC) $(CFLAGS) $(SRC) -o $(EXEC) $(LDFLAGS)
rebuild:clean run

run:all
	./$(EXEC) $(ARGS)
clean:
	rm -f x



