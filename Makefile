CC = gcc
CFLAGS = -Wall -std=c99 -O3 -g
SRCS = tileblaster.c file_funcs.c tile_funcs.c stack.c mem.c dfs.c
OBJS = $(SRCS:.c=.o)
TARGET = tileblaster

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)' > Makefile
