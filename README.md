# Modern Snake Game

A **completely modernized** Snake game implementation in C++ featuring colorful graphics, power-ups, smooth controls, and dynamic gameplay! Transformed from a basic first project into an impressive, fully-functional game.

> **FIXED VERSION AVAILABLE!** The game logic has been completely rewritten and is now fully functional. See [HOWTO.md](HOWTO.md) or [RUN_THIS.txt](RUN_THIS.txt) for quick instructions.

## Quick Start

```bash
# Build and run the game
make run

# Or run directly
./snake_modern_fixed
```

That's it! The game starts immediately with working controls and features.

## Features

### Core Gameplay
- **Classic Snake Mechanics**: Traditional snake movement, growth, and collision
- **Colorful ANSI Graphics**: Vibrant colors for snake, fruits, walls, and power-ups
- **Smart Collision Detection**: Wall and self-collision with optional invincibility
- **Intelligent Spawning**: Prevents fruits and power-ups from spawning inside the snake

### Modern Features
- **Power-Ups System**: 7 different power-ups including speed boost, double points, invincibility, shrink, growth boost, slow-down, and teleport
- **Multiple Game Modes**: Classic, Speed Mode, Survival, and Infinite modes
- **Leaderboard System**: Track top 10 scores with player names and game modes
- **Configuration System**: Customizable settings saved to file
- **Sound Effects**: Audio feedback for eating, power-ups, and game over
- **Interactive Menus**: Beautiful ASCII art menus with keyboard navigation
- **Pause & Resume**: Pause the game anytime with 'P'
- **Dynamic Difficulty**: Game speed increases with score

### Power-Ups
- **S** - Speed Boost (Cyan): Move faster for 5 seconds
- **L** - Slow Down (Blue): Move slower for 5 seconds  
- **D** - Double Points (Yellow): 2x score for 10 seconds
- **I** - Invincible (White): No collision damage for 3 seconds
- **R** - Shrink (Magenta): Reduce snake length by 3
- **G** - Growth Boost (Green): Grow by 5 segments instantly
- **T** - Teleport (Red): Random position on board

## Quick Start

### Prerequisites
- C++ compiler (g++ or clang++)
- Make (optional, for easy building)
- Linux/Unix terminal with ANSI color support

### Building & Running

```bash
# Navigate to project
cd /home/oneknight/projects/Snake-Game

# Build and run (recommended - easiest way)
make run
```

The game will compile and start immediately!

### Manual Build

```bash
# Build
make

# Run
./snake_modern_fixed
```

### Alternative Versions

```bash
# Simple version (basic snake game)
make run-linux

# Classic version
./snake_game_linux
```

## Game Controls

### In-Game
| Key | Action |
|-----|--------|
| `W` | Move Up |
| `A` | Move Left |
| `S` | Move Down |
| `D` | Move Right |
| `P` | Pause/Resume |
| `X` | Quit Game |

### Menus
| Key | Action |
|-----|--------|
| `W/S` or `↑/↓` | Navigate Menu |
| `ENTER` | Select Option |
| `Q` | Quit Menu |

## Gameplay

### How It Works
- Control the snake with WASD keys
- Eat red fruits (X) to grow and score points
- Collect yellow power-ups for special abilities
- Avoid hitting walls or yourself (unless invincible!)
- Game speed increases as your score grows

### Power-Up Effects
Power-ups spawn randomly (20% chance) after eating fruit:
- **S** (Speed): Faster movement temporarily
- **D** (Double): 2x points for a limited time
- **I** (Invincible): Walk through walls and yourself!
- **G** (Growth): Instantly add 3 segments
- **R** (Shrink): Remove 3 segments (useful when too long!)

## Project Structure

```
Snake-Game/
├── modern_snake.cpp      # Modern game with all features
├── main_linux.cpp        # Simple cross-platform version
├── main.cpp              # Original Windows version
├── colors.h              # ANSI color definitions
├── config.h              # Configuration system
├── powerups.h            # Power-ups logic
├── menu.h                # Interactive menu system
├── sound.h               # Sound effects manager
├── leaderboard.h         # Leaderboard tracking
├── Makefile              # Build configuration
├── README.md             # This file
└── DEVELOPMENT.md        # Development guide
```

## Configuration

The game creates a `snake_config.txt` file to store your settings:
- Player name
- Difficulty level
- Sound enabled/disabled
- Colors enabled/disabled
- Power-ups enabled/disabled
- Board dimensions
- Speed multiplier

Edit this file or use the in-game settings menu to customize your experience.

## Leaderboard

High scores are automatically saved to `leaderboard.txt` with:
- Player name
- Score
- Game mode
- Difficulty
- Date

View the leaderboard from the main menu!

## Technical Highlights

### Modern C++ Features
- Object-Oriented Design with clean class architecture
- STL containers (vectors, maps)
- Smart memory management
- Template-based configuration system
- Lambda functions for sound effects
- Chrono library for precise timing

### Graphics & UI
- ANSI escape codes for colors and formatting
- Dynamic screen clearing
- Non-blocking keyboard input
- Smooth terminal-based rendering

### Performance
- Optimized collision detection
- Efficient power-up management
- Memory-safe vector operations
- Minimal CPU usage with smart sleep timing

## Building Different Versions

```bash
# Build everything
make all

# Build only modern version
make

# Build simple Linux version
make snake_game_linux

# Clean all builds
make clean

# Run modern version
make run

# Run simple version  
make run-linux
```

## Development

Want to contribute or modify the game? Check out [DEVELOPMENT.md](DEVELOPMENT.md) for:
- Development environment setup
- Code architecture overview
- Adding new features
- Testing guidelines
- Contributing process

### Quick Modification Ideas
- Add new power-up types in `powerups.h`
- Create new game modes in `modern_snake.cpp`
- Customize colors in `colors.h`
- Add difficulty presets in `config.h`

## Troubleshooting

### Game doesn't display colors
Your terminal may not support ANSI colors. Try:
```bash
echo $TERM  # Should show something like 'xterm-256color'
```

### Controls not responding
The terminal needs to support non-blocking input. Most modern terminals do.

### Sound not working
Sound uses system beep. Enable it with:
```bash
# Unmute system beep
xset b on
```

### Game runs too fast/slow
Adjust the `speed_multiplier` in `snake_config.txt` or use the settings menu.

## Screenshots

```
################################
#                              #
#    S        D                #
#                              #
#         OOOOO                #
#             o                #
#             X                #
#                    I         #
#                              #
################################
Score: 150
High Score: 280
DOUBLE POINTS!
```

## Comparison: Before vs After

### Before (Basic Version)
- Plain ASCII graphics
- No colors
- Single game mode
- Basic score tracking
- Simple controls

### After (Modern Version)
- Colorful ANSI graphics
- 7 unique power-ups
- 4 game modes
- Full leaderboard system
- Interactive menus
- Sound effects
- Configuration system
- Multiple difficulty levels

## License

This project is open source. Feel free to use, modify, and distribute.

## Acknowledgments

Transformed from a basic first project into a modern, feature-rich game showcasing:
- Advanced C++ programming
- Game development patterns
- System programming (terminal control)
- File I/O and data persistence
- User interface design
- Object-oriented architecture

---

**From a simple first project to an impressive portfolio piece!**

Made with C++ | Cross-platform | No external dependencies