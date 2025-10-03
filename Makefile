# Makefile for Snake Game
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
SDL_FLAGS = $(shell sdl2-config --cflags --libs) -lSDL2_ttf -lSDL2_mixer -lSDL2_image
TARGET = snake_game
LINUX_TARGET = snake_game_linux
MODERN_TARGET = modern_snake
GUI_TARGET = snake_gui
SOURCE = main.cpp
LINUX_SOURCE = main_linux.cpp
MODERN_SOURCE = modern_snake.cpp
GUI_SOURCE = snake_gui.cpp

# Default target - GUI version if SDL2 available, otherwise terminal
all: check_sdl

# Check for SDL2 and build accordingly
check_sdl:
	@if command -v sdl2-config >/dev/null 2>&1; then \
		echo "SDL2 found - building GUI version..."; \
		$(MAKE) $(GUI_TARGET); \
	else \
		echo "SDL2 not found - building terminal version..."; \
		echo "Run './install_sdl.sh' to install SDL2 for GUI version"; \
		$(MAKE) snake_modern_fixed; \
	fi

# Build the GUI version (requires SDL2)
$(GUI_TARGET): $(GUI_SOURCE)
	$(CXX) $(CXXFLAGS) -o $(GUI_TARGET) $(GUI_SOURCE) $(SDL_FLAGS)

# Build the working modern terminal version
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
	rm -f $(TARGET) $(LINUX_TARGET) $(MODERN_TARGET) $(GUI_TARGET) snake_modern_fixed

# Install SDL2 dependencies for GUI version
install-deps:
	@echo "Installing SDL2 libraries..."
	@./install_sdl.sh

# Run the best available version
run: all
	@if [ -f $(GUI_TARGET) ]; then \
		echo "Running GUI version..."; \
		./$(GUI_TARGET); \
	else \
		echo "Running terminal version..."; \
		./snake_modern_fixed; \
	fi

# Run GUI version specifically
run-gui: $(GUI_TARGET)
	./$(GUI_TARGET)

# Run terminal version specifically
run-terminal: snake_modern_fixed
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