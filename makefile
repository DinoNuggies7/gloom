# Define the compiler and compilation flags
CC = gcc
CCACHE = ccache
CFLAGS = -g
LIBS = -lm -lSDL2 -lSDL2_image -lcjson

# Directories
SRC_DIR = src
BUILD_DIR = build

# List of source files and corresponding object files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

# The default target is to build all object files
all: $(OBJ_FILES)
	$(CCACHE) $(CC) $(BUILD_DIR)/*.o $(LIBS)
	./a.out

# Rule to compile a .c file to a .o file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CCACHE) $(CC) $(CFLAGS) -c $< -o $@

# Clean up the object files
clean:
	rm -f $(BUILD_DIR)/*.o

# PHONY target to force the "clean" target to run even if a file named "clean" exists
.PHONY: clean
