# Makefile for Snake Game
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
TARGET = snake_game
LINUX_TARGET = snake_game_linux
MODERN_TARGET = modern_snake
SOURCE = main.cpp
LINUX_SOURCE = main_linux.cpp
MODERN_SOURCE = modern_snake.cpp

# Default target
all: snake_modern_fixed

# Build the working modern version (recommended)
snake_modern_fixed: snake_modern_fixed.cpp
	$(CXX) $(CXXFLAGS) -o snake_modern_fixed snake_modern_fixed.cpp

# Build the complex modern version (has menu issues)
$(MODERN_TARGET): $(MODERN_SOURCE) colors.h config.h powerups.h menu.h sound.h leaderboard.h
	$(CXX) $(CXXFLAGS) -o $(MODERN_TARGET) $(MODERN_SOURCE)

# Build the Linux version (cross-platform)
$(LINUX_TARGET): $(LINUX_SOURCE)
	$(CXX) $(CXXFLAGS) -o $(LINUX_TARGET) $(LINUX_SOURCE)

# Build the original Windows version (if on Windows)
$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCE)

# Clean build artifacts
clean:
	rm -f $(TARGET) $(LINUX_TARGET) $(MODERN_TARGET) snake_modern_fixed

# Install dependencies (if needed)
install-deps:
	@echo "No external dependencies required for basic version"

# Run the working modern version (recommended)
run: snake_modern_fixed
	./snake_modern_fixed

# Run the Linux version
run-linux: $(LINUX_TARGET)
	./$(LINUX_TARGET)

# Run the original Windows version
run-windows: $(TARGET)
	./$(TARGET)

# Debug build
debug: CXXFLAGS += -g -DDEBUG
debug: $(TARGET)

# Release build
release: CXXFLAGS += -DNDEBUG
release: $(TARGET)

.PHONY: all clean install-deps run debug release