# Anas Rchid - fall 2016
# Knight's Tour Makefile
# License: LGPL (v3)

CC = gcc
CFLAGS = -Wall -Werror -ggdb -std=c99
OBJ = kt.o
SRC = kt.c
LDFLAGS = #

all:
	$(CC) $(CFLAGS) $(SRC) -o x $(LDFLAGS)
clean:
	rm -f x


