#!/bin/bash

# Test script for Snake Game
echo "🐍 Testing Snake Game..."
echo "=========================="

# Check if executable exists
if [ ! -f "./snake_game_linux" ]; then
    echo "❌ Executable not found. Building..."
    make
    if [ $? -ne 0 ]; then
        echo "❌ Build failed!"
        exit 1
    fi
fi

echo "✅ Build successful!"
echo "🎮 Game executable: ./snake_game_linux"
echo ""
echo "Controls:"
echo "  WASD - Move snake"
echo "  P    - Pause"
echo "  R    - Restart (when paused)"
echo "  X    - Quit"
echo ""
echo "To run the game: ./snake_game_linux"
echo "Or use: make run"
echo ""
echo "📁 Project files:"
ls -la *.cpp *.md Makefile score.txt 2>/dev/null
echo ""
echo "🚀 Ready for development!"