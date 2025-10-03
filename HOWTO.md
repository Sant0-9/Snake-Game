# How to Run Your Modern Snake Game

## WORKING VERSION - FIXED GAME LOGIC

The game logic has been fixed! Here's how to run it:

## Quick Start (Recommended)

```bash
cd /home/oneknight/projects/Snake-Game

# Build and run (easiest way)
make run
```

The game will start immediately with proper controls and logic!

## Alternative: Manual Build

```bash
# Build
g++ -std=c++11 -Wall -Wextra -O2 -o snake_modern_fixed snake_modern_fixed.cpp

# Run
./snake_modern_fixed
```

## Game Features

### Working Features
✓ **Smooth Controls** - WASD movement that responds properly
✓ **Colorful Graphics** - Green snake, red fruit, yellow power-ups
✓ **Power-ups** - 5 types that actually work!
✓ **Score Tracking** - Displays current and high score
✓ **Progressive Speed** - Gets faster as you score
✓ **Play Again** - Restart without closing the program

### Power-Ups (Yellow Letters)
- **S** = Speed Boost (faster movement)
- **D** = Double Points (2x score for limited time)
- **I** = Invincible (walk through walls and yourself)
- **G** = Growth (instantly add 3 segments)
- **R** = Shrink (remove 3 segments)

## Controls

### During Game
- **W** = Move Up
- **A** = Move Left
- **S** = Move Down
- **D** = Move Right
- **P** = Pause (press any key to continue)
- **X** = Quit game

### After Game Over
- **Y** = Play again
- **N** = Exit

## What Was Fixed

### Issues Found
1. ❌ Terminal input wasn't working properly (getchar() blocking)
2. ❌ Menu system was causing the game to exit immediately
3. ❌ Configuration files causing initialization issues
4. ❌ Complex header dependencies

### Solutions Applied
1. ✅ Fixed non-blocking terminal input with proper fcntl()
2. ✅ Removed complex menu system - game starts immediately
3. ✅ Self-contained game logic in single file
4. ✅ Simplified terminal setup/restore
5. ✅ Fixed collision detection
6. ✅ Fixed power-up spawn and collision
7. ✅ Proper tail movement logic

## Versions Available

### 1. snake_modern_fixed (RECOMMENDED - WORKING)
```bash
make run
# OR
./snake_modern_fixed
```
**Features:** Colors, power-ups, proper controls, smooth gameplay

### 2. snake_game_linux (Simple Classic)
```bash
make run-linux
# OR  
./snake_game_linux
```
**Features:** Basic snake game, no colors, classic mode

### 3. main_linux.cpp (Original)
Still available if needed

## Troubleshooting

### Issue: Game doesn't respond to keys
**Solution:** The fixed version handles this correctly now. Just use `make run`

### Issue: Colors not showing
**Solution:** Your terminal may not support ANSI colors. The game still works!

### Issue: Game too fast/slow
**Solution:** Speed automatically adjusts with your score. Start slow, gets faster!

### Issue: Can't see the snake
**Solution:** Make sure your terminal is at least 32x25 characters

## Gameplay Tips

1. **Start Slow**: Wait at the title screen, then start when ready
2. **Collect Power-ups**: Yellow letters give you special abilities
3. **Use Invincible**: The 'I' power-up lets you phase through walls temporarily
4. **Double Points**: Stack 'D' power-up with lots of fruit eating
5. **Don't Shrink Too Much**: 'R' power-up can make you too small

## Technical Details

### What Makes This Work
- **Non-blocking Input**: fcntl() with O_NONBLOCK
- **Proper Terminal Setup**: termios configuration
- **Fixed Game Loop**: draw → input → logic → sleep
- **Collision Detection**: Proper boundary and self-collision checks
- **Memory Management**: Vector-based tail with proper sizing

### Performance
- Runs at ~10 FPS initially
- Speed increases with score
- Minimal CPU usage (uses usleep)
- No memory leaks

## Score System

- **Regular Fruit**: 10 points
- **Double Points Active**: 20 points per fruit
- **High Score**: Automatically tracked across play sessions

## Next Steps

1. Play the game: `make run`
2. Beat your high score!
3. Try all power-ups
4. See how fast you can go

---

**The game is now fully functional with proper logic and controls!**

Enjoy playing! 🎮