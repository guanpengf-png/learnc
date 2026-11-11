CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -g
TARGET = contacts

.PHONY: all run clean

all: $(TARGET)

$(TARGET): contacts.c
	$(CC) $(CFLAGS) $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) contacts.txt
