# Name of the binary
TARGET = detect_dups

# Compiler and flags
CC = gcc
CFLAGS = #-Wall -Wextra -O2

# Default rule
all: $(TARGET)

# Build rule
$(TARGET): detect_dups.c
	$(CC) $(CFLAGS) -o $(TARGET) detect_dups.c

# Clean rule
clean:
	rm -f $(TARGET)
