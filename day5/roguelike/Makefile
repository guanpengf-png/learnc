CC      = cc
CFLAGS  = -std=c11 -Wall -Wextra -O2 -I.
SRCS    = main.c game/game.c \
          ds/vector.c ds/list.c ds/stack.c ds/queue.c ds/hash.c \
          ds/heap.c ds/uf.c ds/graph.c ds/path.c ds/mst.c ds/bsp.c \
          ds/bst.c ds/avl.c ds/sort.c ds/search.c ds/dp.c ds/kmp.c \
          ds/trie.c ds/backtrack.c ds/greedy.c
OBJS    = $(SRCS:.c=.o)
TARGET  = dungeon

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
