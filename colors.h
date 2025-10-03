#ifndef COLORS_H
#define COLORS_H

// ANSI Color Codes for terminal output
namespace Colors {
    // Reset
    const char* RESET = "\033[0m";
    
    // Regular Colors
    const char* BLACK = "\033[0;30m";
    const char* RED = "\033[0;31m";
    const char* GREEN = "\033[0;32m";
    const char* YELLOW = "\033[0;33m";
    const char* BLUE = "\033[0;34m";
    const char* MAGENTA = "\033[0;35m";
    const char* CYAN = "\033[0;36m";
    const char* WHITE = "\033[0;37m";
    
    // Bold Colors
    const char* BOLD_BLACK = "\033[1;30m";
    const char* BOLD_RED = "\033[1;31m";
    const char* BOLD_GREEN = "\033[1;32m";
    const char* BOLD_YELLOW = "\033[1;33m";
    const char* BOLD_BLUE = "\033[1;34m";
    const char* BOLD_MAGENTA = "\033[1;35m";
    const char* BOLD_CYAN = "\033[1;36m";
    const char* BOLD_WHITE = "\033[1;37m";
    
    // Background Colors
    const char* BG_BLACK = "\033[40m";
    const char* BG_RED = "\033[41m";
    const char* BG_GREEN = "\033[42m";
    const char* BG_YELLOW = "\033[43m";
    const char* BG_BLUE = "\033[44m";
    const char* BG_MAGENTA = "\033[45m";
    const char* BG_CYAN = "\033[46m";
    const char* BG_WHITE = "\033[47m";
    
    // Special Effects
    const char* BLINK = "\033[5m";
    const char* UNDERLINE = "\033[4m";
    const char* REVERSE = "\033[7m";
}

// Game-specific color schemes
namespace GameColors {
    const char* SNAKE_HEAD = Colors::BOLD_GREEN;
    const char* SNAKE_BODY = Colors::GREEN;
    const char* FRUIT = Colors::BOLD_RED;
    const char* WALL = Colors::BOLD_WHITE;
    const char* BACKGROUND = Colors::BLACK;
    const char* SCORE = Colors::BOLD_CYAN;
    const char* MENU_TITLE = Colors::BOLD_MAGENTA;
    const char* MENU_OPTION = Colors::YELLOW;
    const char* MENU_SELECTED = Colors::BOLD_YELLOW;
    const char* GAME_OVER = Colors::BOLD_RED;
    const char* SUCCESS = Colors::BOLD_GREEN;
}

#endif // COLORS_H