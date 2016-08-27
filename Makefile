# ANAS RCHID - FALL 2016
# KNIGHT'S TOUR MAKEFILE
# LICENSE: LGPL (v3)
#
CC = gcc
CFLAGS = -Wall -Werror -ggdb -std=c99
OBJ = kt.o
SRC = kt.c
LDFLAGS = -lpopt
ARGS = -dRA -w 5 -c 5
EXEC = x
#
all:
	$(CC) $(CFLAGS) $(SRC) -o $(EXEC) $(LDFLAGS)
#
rebuild:clean run
#
run:all
	./$(EXEC) $(ARGS)
#
clean:
	rm -f x
# END


