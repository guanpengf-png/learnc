CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -g
TARGET = student

.PHONY: all run clean

all: $(TARGET)

$(TARGET): student.c
	$(CC) $(CFLAGS) $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) students.txt
