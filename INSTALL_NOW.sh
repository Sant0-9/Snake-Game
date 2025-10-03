#!/bin/bash

echo "================================================"
echo "  Snake Game GUI - Installation & Build"
echo "================================================"
echo ""
echo "Step 1: Installing SDL2 libraries..."
echo "You may be prompted for your password."
echo ""

# Install SDL2
sudo apt-get update && \
sudo apt-get install -y \
    libsdl2-dev \
    libsdl2-ttf-dev \
    libsdl2-mixer-dev \
    libsdl2-image-dev \
    fonts-dejavu

if [ $? -ne 0 ]; then
    echo ""
    echo "ERROR: Failed to install SDL2"
    echo "Please run manually:"
    echo "  sudo apt-get update"
    echo "  sudo apt-get install -y libsdl2-dev libsdl2-ttf-dev libsdl2-mixer-dev libsdl2-image-dev fonts-dejavu"
    exit 1
fi

echo ""
echo "================================================"
echo "  SDL2 Installed Successfully!"
echo "================================================"
echo ""
echo "Step 2: Building GUI version..."
echo ""

# Build the GUI version
cd /home/oneknight/projects/Snake-Game
make clean
make snake_gui

if [ $? -ne 0 ]; then
    echo ""
    echo "ERROR: Build failed"
    echo "Check for compilation errors above"
    exit 1
fi

echo ""
echo "================================================"
echo "  BUILD SUCCESSFUL!"
echo "================================================"
echo ""
echo "Your GUI Snake game is ready to play!"
echo ""
echo "To run it:"
echo "  ./snake_gui"
echo ""
echo "Or:"
echo "  make run-gui"
echo ""
echo "================================================"
