TARGET = bin/cUsage

# Clang:
CC = clang
CFLAGS = -std=c11 -Weverything -pthread

# GCC:
# CC = gcc
# CFLAGS = -std=c11 -Wall -Wextra -pthread

# Dirs and includes
SRC_DIR = src
INCLUDE_DIR = include
INCLUDES = -I$(INCLUDE_DIR)
BUILD_DIR = build

# Sources:
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))

# Tests:
TEST_SRCS = $(wildcard tests/test_*.c)
TEST_EXECUTABLES = $(patsubst tests/%.c,$(BUILD_DIR)/%,$(TEST_SRCS))

all: $(TARGET)
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(BUILD_DIR)/*.o $(TEST_EXECUTABLES)

test: $(TEST_EXECUTABLES)
	@for test_exec in $(TEST_EXECUTABLES); do \
		echo "Running $$test_exec"; \
		$$test_exec; \
	done

$(BUILD_DIR)/%: tests/%.c $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDES) $(filter-out $(BUILD_DIR)/main.o,$(OBJECTS)) $< -o $@
