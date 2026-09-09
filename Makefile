CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude
TARGET = c-image-filters

SRCS = src/main.c src/image.c src/filters.c
OBJS = $(SRCS:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
