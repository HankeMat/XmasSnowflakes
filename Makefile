# Update the source and library directories
SRC_DIR = src
LIB_DIR = lib

# List source files with paths
SRC_FILES = $(SRC_DIR)/main.c $(SRC_DIR)/sdl.c

# Generate object file names from source file names
OBJFILES = $(patsubst %.c,%.o,$(SRC_FILES))

CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -Wextra -g -I$(LIB_DIR)/include
LDFLAGS = -L$(LIB_DIR)/lib -lSDL2

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

all: $(OBJFILES)
	$(CC) $(CFLAGS) $(OBJFILES) -o main $(LDFLAGS)

dep:
	$(CC) -MM $(SRC_FILES) >dep.list

-include dep.list

clean:
	rm -f $(OBJFILES) main dep.list
