# Quick Start Guide

## How to Run Your Modern Snake Game

### Option 1: Modern Version (Recommended)

The modern version has menus, power-ups, colors, and leaderboards!

```bash
# Navigate to your project
cd /home/oneknight/projects/Snake-Game

# Build the game
make

# Run it!
./modern_snake
```

**Note:** The modern version starts with an interactive menu. Navigate with W/S and press ENTER to select.

### Option 2: Simple Version

The simple version starts playing immediately - no menus, just classic Snake!

```bash
# Build and run the simple version
make run-linux
```

This will compile and immediately start the game.

## Controls

### Playing the Game
- **W** = Move Up
- **A** = Move Left  
- **S** = Move Down
- **D** = Move Right
- **P** = Pause
- **X** = Quit

### In Menus (Modern Version Only)
- **W/S** = Navigate up/down
- **ENTER** = Select option
- **Q** = Go back/quit

## What You'll See

### Modern Version
1. Welcome screen with ASCII art logo
2. Main menu with options:
   - Start Game
   - Game Modes
   - Settings
   - Leaderboard
   - About
   - Exit

### Simple Version
- Jumps straight into the game
- No menus or power-ups
- Classic Snake gameplay

## Power-Ups (Modern Version Only)

During gameplay, colorful letters will appear on the board:
- **S** (Cyan) = Speed Boost
- **L** (Blue) = Slow Down
- **D** (Yellow) = Double Points
- **I** (White) = Invincible
- **R** (Magenta) = Shrink
- **G** (Green) = Growth Boost
- **T** (Red) = Teleport

## Troubleshooting

### Problem: "Command not found: make"
**Solution:** Build manually:
```bash
g++ -std=c++11 -Wall -Wextra -O2 -o modern_snake modern_snake.cpp
./modern_snake
```

### Problem: Modern version closes immediately
**Solution:** Use the simple version instead:
```bash
g++ -std=c++11 -Wall -Wextra -O2 -o snake_game_linux main_linux.cpp
./snake_game_linux
```

### Problem: No colors showing
**Solution:** Your terminal might not support ANSI colors. The game will still work, just without colors.

### Problem: Can't control the snake
**Solution:** Make sure you press ENTER at the welcome screen first (modern version), or start the simple version instead.

## Quick Tips

1. **First time playing?** Use the simple version to get comfortable with controls
2. **Want the full experience?** Use the modern version and explore all the features
3. **Customize the game:** Edit `snake_config.txt` after running once
4. **Check your high scores:** Choose "Leaderboard" from the main menu

## File Locations

After running, these files will be created:
- `snake_config.txt` - Your settings
- `leaderboard.txt` - High scores
- `score.txt` - Simple version scores

## Need More Help?

- Full documentation: See [README.md](README.md)
- Development info: See [DEVELOPMENT.md](DEVELOPMENT.md)
- File structure: Run `ls -la` in the project directory

---

**Have fun playing your modern Snake game!**