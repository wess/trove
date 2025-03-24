# Compiler and flags
CC = gcc
CFLAGS_DEBUG = -Wall -Wextra -std=c99 -g -O0 -Isrc
CFLAGS_RELEASE = -Wall -Wextra -std=c99 -O2 -DNDEBUG -Isrc

# Directories
BUILD_DIR   = build
DEBUG_DIR   = $(BUILD_DIR)/debug
RELEASE_DIR = $(BUILD_DIR)/release

LIB_NAME = libtrove.a

# Pattern rule for object files in debug build
$(DEBUG_DIR)/%.o: src/%.c src/trove.h | $(DEBUG_DIR)
	$(CC) $(CFLAGS_DEBUG) -c $< -o $@

# Build the static library from trove.o in debug build
$(DEBUG_DIR)/$(LIB_NAME): $(DEBUG_DIR)/trove.o | $(DEBUG_DIR)
	ar rcs $(DEBUG_DIR)/$(LIB_NAME) $(DEBUG_DIR)/trove.o

# Link testtrove executable in debug build into the debug directory
$(DEBUG_DIR)/testtrove: $(DEBUG_DIR)/main.o $(DEBUG_DIR)/$(LIB_NAME) | $(DEBUG_DIR)
	$(CC) $(CFLAGS_DEBUG) $(DEBUG_DIR)/main.o -L$(DEBUG_DIR) -ltrove -o $@

# Pattern rule for object files in release build
$(RELEASE_DIR)/%.o: src/%.c src/trove.h | $(RELEASE_DIR)
	$(CC) $(CFLAGS_RELEASE) -c $< -o $@

# Build the static library from trove.o in release build
$(RELEASE_DIR)/$(LIB_NAME): $(RELEASE_DIR)/trove.o | $(RELEASE_DIR)
	ar rcs $(RELEASE_DIR)/$(LIB_NAME) $(RELEASE_DIR)/trove.o

# Link main executable in release build
$(RELEASE_DIR)/main: $(RELEASE_DIR)/main.o $(RELEASE_DIR)/$(LIB_NAME) | $(RELEASE_DIR)
	$(CC) $(CFLAGS_RELEASE) $(RELEASE_DIR)/main.o -L$(RELEASE_DIR) -ltrove -o $@

# Create build directories if they don't exist
$(DEBUG_DIR):
	mkdir -p $(DEBUG_DIR)

$(RELEASE_DIR):
	mkdir -p $(RELEASE_DIR)

# Phony targets
.PHONY: all debug release clean testtrove

# Default target: build both debug and release versions
all: debug

# Debug build: create testtrove in the debug directory
debug: $(DEBUG_DIR)/testtrove

# Release build remains unchanged
release: $(RELEASE_DIR)/main

# For convenience, "make testtrove" builds the debug executable
testtrove: $(DEBUG_DIR)/testtrove

clean:
	rm -rf $(BUILD_DIR)