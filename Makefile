TARGET = bin/cUsage

# Clang:
# CC = clang
# CFLAGS = -std=c11 -Weverything -pthread

# GCC:
CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pthread

# Dirs and includes
SRC_DIR = src
INCLUDE_DIR = include
INCLUDES = -I$(INCLUDE_DIR)
BUILD_DIR = build

# Sources:
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))

all: $(TARGET)
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) test_$(TARGET) $(BUILD_DIR)/*.o

