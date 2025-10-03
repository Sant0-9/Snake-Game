#!/bin/bash

echo "=========================================="
echo "  Installing SDL2 for Snake Game GUI"
echo "=========================================="
echo ""

# Detect package manager
if command -v apt-get &> /dev/null; then
    echo "Detected: Ubuntu/Debian"
    sudo apt-get update
    sudo apt-get install -y libsdl2-dev libsdl2-ttf-dev libsdl2-mixer-dev libsdl2-image-dev
elif command -v dnf &> /dev/null; then
    echo "Detected: Fedora"
    sudo dnf install -y SDL2-devel SDL2_ttf-devel SDL2_mixer-devel SDL2_image-devel
elif command -v pacman &> /dev/null; then
    echo "Detected: Arch Linux"
    sudo pacman -S --noconfirm sdl2 sdl2_ttf sdl2_mixer sdl2_image
else
    echo "Could not detect package manager"
    echo "Please install SDL2 manually:"
    echo "  - SDL2"
    echo "  - SDL2_ttf"
    echo "  - SDL2_mixer"
    echo "  - SDL2_image"
    exit 1
fi

echo ""
echo "=========================================="
echo "  SDL2 Installation Complete!"
echo "=========================================="
echo ""
echo "Now you can build the GUI version:"
echo "  make gui"
echo "  ./snake_gui"
