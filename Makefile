TARGET = output/cUsage

# Clang:
# CC = clang
# CFLAGS = -std=c11 -Weverything -pthread

# GCC:
CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pthread

# Dirs and includes"
SRC_DIR = src
INCLUDE_DIR = include
INCLUDES = -I$(INCLUDE_DIR)

# Sources:
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJECTS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) test_$(TARGET) $(OBJECTS)

