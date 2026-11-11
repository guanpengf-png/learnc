CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -g
TARGET = mempool

.PHONY: all run interactive clean

all: $(TARGET)

$(TARGET): mempool.c
	$(CC) $(CFLAGS) $< -o $@

run: $(TARGET)
	./$(TARGET)

interactive: $(TARGET)
	./$(TARGET) -i

clean:
	rm -f $(TARGET)
