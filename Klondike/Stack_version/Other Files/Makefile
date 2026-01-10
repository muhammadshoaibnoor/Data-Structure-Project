# Makefile for Klondike Solitaire Game
# Supports Windows, Linux, and macOS

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -O2

# Source files
SOURCES = klondike.cpp

# Detect operating system
ifeq ($(OS),Windows_NT)
    # Windows
    TARGET = klondike.exe
    LIBS = -lraylib -lopengl32 -lgdi32 -lwinmm
    RM = del /Q
    RM_TARGET = if exist $(TARGET) del /Q $(TARGET)
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        # Linux
        TARGET = klondike
        LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
        RM = rm -f
        RM_TARGET = $(RM) $(TARGET)
    endif
    ifeq ($(UNAME_S),Darwin)
        # macOS
        TARGET = klondike
        LIBS = -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
        RM = rm -f
        RM_TARGET = $(RM) $(TARGET)
    endif
endif

# Default target
all: $(TARGET)

# Build target
$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET) $(LIBS)

# Run the game
run: $(TARGET)
	./$(TARGET)

# Clean build files
clean:
	$(RM_TARGET)

# Rebuild
rebuild: clean all

# Help
help:
	@echo "Klondike Solitaire - Makefile"
	@echo "Available targets:"
	@echo "  make          - Build the game"
	@echo "  make run      - Build and run the game"
	@echo "  make clean    - Remove built files"
	@echo "  make rebuild  - Clean and rebuild"
	@echo "  make help     - Show this help message"

.PHONY: all run clean rebuild help
