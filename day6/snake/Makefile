CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -g
TARGET = snake

.PHONY: all run clean

all: $(TARGET)

$(TARGET): snake.c
	$(CC) $(CFLAGS) $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
