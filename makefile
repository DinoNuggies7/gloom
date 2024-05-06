# Define the compiler and compilation flags
CCACHE = ccache
CFLAGS = -Wall -Ofast
LIBS = -lm -lSDL2 -lSDL2_image -lSDL2_mixer

# Directories
SRC_DIR = src
BUILD_DIR = build

# List of source files and corresponding object files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

# The default target is to build all object files
all: clean $(OBJ_FILES)
	$(CCACHE) $(CC) $(BUILD_DIR)/*.o $(LIBS) -o gloom

# Rule to compile a .c file to a .o file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CCACHE) $(CC) $(CFLAGS) -c $< -o $@

# Clean up the object files
clean:
	rm -f $(BUILD_DIR)/*.o
	mkdir -p $(BUILD_DIR)

# PHONY target to force the "clean" target to run even if a file named "clean" exists
.PHONY: clean
