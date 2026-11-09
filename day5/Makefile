CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2

.PHONY: all clean
all: dungeon

dungeon: main.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f dungeon
