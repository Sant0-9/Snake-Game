# Snake Game - GUI Version Setup

## Transform Your Terminal Game into a Real App!

Your Snake game now has a **beautiful GUI version** with actual windows, smooth graphics, and modern visuals!

## Quick Setup (3 Steps)

### Step 1: Install SDL2

```bash
cd /home/oneknight/projects/Snake-Game
./install_sdl.sh
```

Or install manually:
```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install -y libsdl2-dev libsdl2-ttf-dev libsdl2-mixer-dev libsdl2-image-dev

# Fedora
sudo dnf install -y SDL2-devel SDL2_ttf-devel SDL2_mixer-devel SDL2_image-devel

# Arch Linux
sudo pacman -S sdl2 sdl2_ttf sdl2_mixer sdl2_image
```

### Step 2: Build the GUI Version

```bash
make clean
make snake_gui
```

### Step 3: Run It!

```bash
./snake_gui
```

Or simply:
```bash
make run-gui
```

## GUI Features

### Visual Improvements
- ✨ **Actual Window Application** - Real GUI window, not terminal!
- 🎨 **Smooth Graphics** - 800x600 resolution, 20x20 grid
- 🎯 **Circular Elements** - Snake head and fruit are circles
- 🌈 **Beautiful Colors** - Dark blue background, vibrant snake and fruit
- ✨ **Grid Lines** - Clean visual grid
- 💫 **Glow Effects** - Snake glows when invincible
- 📊 **HUD Display** - Score, high score, and status always visible

### Gameplay Features
- 🎮 **Smooth Controls** - Arrow keys or WASD
- ⚡ **Power-ups** - 5 types with visual indicators
- 🏆 **High Score Tracking** - Saved between games
- ⏸️ **Pause System** - Press SPACE to pause/resume
- 🔄 **Instant Restart** - Press R after game over
- 🎯 **Progressive Speed** - Gets faster as you score
- 🛡️ **Invincibility** - Walk through walls with glow effect
- 2️⃣ **Double Points** - 2x score multiplier

## Controls

### In-Game
| Key | Action |
|-----|--------|
| `↑` or `W` | Move Up |
| `↓` or `S` | Move Down |
| `←` or `A` | Move Left |
| `→` or `D` | Move Right |
| `SPACE` | Pause/Resume |
| `ESC` | Quit Game |

### After Game Over
| Key | Action |
|-----|--------|
| `R` | Restart Game |
| `ESC` | Exit |

## Power-Ups

When you eat a fruit, there's a 25% chance a power-up spawns:

- 🚀 **Speed Boost** - Faster movement
- 2️⃣ **Double Points** - 2x score for 5 seconds (yellow indicator)
- 🛡️ **Invincible** - Walk through walls and yourself for 3 seconds (glow effect!)
- ➕ **Growth** - Instantly add 3 segments
- ➖ **Shrink** - Remove 3 segments (helpful when too long!)

Power-ups expire after 10 seconds if not collected.

## Technical Details

### Window Specifications
- **Resolution**: 800x600 pixels
- **Grid Size**: 40x30 cells (20px per cell)
- **Frame Rate**: 60 FPS rendering, 10 FPS game logic
- **Graphics**: SDL2 with hardware acceleration

### Color Scheme
- **Background**: Dark Blue (#141428)
- **Snake Head**: Bright Green (#32FF32)
- **Snake Body**: Green (#1EC81E)
- **Fruit**: Bright Red (#FF3232)
- **Power-ups**: Golden Yellow (#FFC832)
- **Grid Lines**: Subtle Gray (#282840)

### Features
- Hardware-accelerated rendering
- VSync for smooth visuals
- Anti-aliased text
- No screen tearing
- Responsive controls

## Troubleshooting

### Problem: SDL2 not found
```bash
# Run the install script
./install_sdl.sh

# Or check if it's installed
sdl2-config --version
```

### Problem: Font not loading
The game tries these fonts in order:
1. `/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf`
2. `/usr/share/fonts/TTF/DejaVuSans.ttf`

Install DejaVu fonts:
```bash
# Ubuntu/Debian
sudo apt-get install fonts-dejavu

# Fedora
sudo dnf install dejavu-sans-fonts

# Arch
sudo pacman -S ttf-dejavu
```

### Problem: Compilation errors
Make sure you have all dependencies:
```bash
sudo apt-get install build-essential libsdl2-dev libsdl2-ttf-dev
```

### Problem: Black screen
Try running with software rendering:
```bash
SDL_RENDER_DRIVER=software ./snake_gui
```

## Comparison: Terminal vs GUI

### Terminal Version
```
################################
#                              #
#         O                    #
#         o                    #
#         o     X              #
#                              #
################################
Score: 30
```

### GUI Version
```
+------------------------------------------+
|  Modern Snake Game - GUI Edition         |
+------------------------------------------+
|                                          |
|    🟢 ● (smooth green circles)          |
|       ●                                  |
|       ●    🔴 ● (red fruit)             |
|                                          |
|  💛 ● (power-up)                        |
|                                          |
|  Score: 30    High Score: 150           |
+------------------------------------------+
```

## Build Options

```bash
# Build GUI version
make snake_gui

# Build terminal version
make snake_modern_fixed

# Build both
make all

# Clean everything
make clean

# Install dependencies
make install-deps
```

## Smart Build System

The Makefile automatically detects SDL2:
- ✅ **SDL2 installed**: Builds GUI version by default
- ❌ **SDL2 not installed**: Falls back to terminal version
- 💡 **Run `make run`**: Automatically runs the best available version!

## Next Steps

1. **Install SDL2**: Run `./install_sdl.sh`
2. **Build**: Run `make snake_gui`
3. **Play**: Run `./snake_gui`
4. **Enjoy**: Experience Snake in a real window!

## Screenshots

The GUI version features:
- Smooth circular snake and fruit
- Grid lines for visual clarity
- Always-visible score display
- Glow effect when invincible
- Status indicators for active power-ups
- Clean, modern design
- Dark theme (easy on the eyes)

## Future GUI Enhancements

Want to make it even better? Ideas:
- [ ] Add sprite graphics (images instead of circles)
- [ ] Background music
- [ ] Sound effects for eating and power-ups
- [ ] Particle effects
- [ ] Multiple themes/skins
- [ ] Animated menus
- [ ] High score leaderboard screen
- [ ] Achievements system

---

**You now have a real GUI application, not just a terminal game!** 🎮

Enjoy your modern Snake game app!