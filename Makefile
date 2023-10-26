CC = gcc
CFLAGS = -Wall -std=c99 -O3
SRCS = tileblaster.c file_funcs.c tile_funcs.c stack.c mem.c vars.c
TARGET = tileblaster

all: $(TARGET)

$(TARGET): 
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET) Makefile
