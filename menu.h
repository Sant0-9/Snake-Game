#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <vector>
#include <string>
#include "colors.h"
#include "config.h"

class Menu {
private:
    std::vector<std::string> options;
    int selectedIndex;
    std::string title;
    
public:
    Menu(const std::string& title) : selectedIndex(0), title(title) {}
    
    void addOption(const std::string& option) {
        options.push_back(option);
    }
    
    void display() {
        system("clear");
        
        // Display title
        std::cout << GameColors::MENU_TITLE << title << Colors::RESET << std::endl;
        std::cout << std::string(title.length(), '=') << std::endl << std::endl;
        
        // Display options
        for (size_t i = 0; i < options.size(); i++) {
            if (i == selectedIndex) {
                std::cout << GameColors::MENU_SELECTED << "> " << options[i] << Colors::RESET << std::endl;
            } else {
                std::cout << GameColors::MENU_OPTION << "  " << options[i] << Colors::RESET << std::endl;
            }
        }
        
        std::cout << std::endl << GameColors::MENU_OPTION << "Use W/S or UP/DOWN to navigate, ENTER to select" << Colors::RESET << std::endl;
    }
    
    int getSelection() {
        char key;
        while (true) {
            key = getchar();
            
            switch (key) {
                case 'w':
                case 'W':
                case 65: // Up arrow
                    selectedIndex = (selectedIndex - 1 + options.size()) % options.size();
                    display();
                    break;
                case 's':
                case 'S':
                case 66: // Down arrow
                    selectedIndex = (selectedIndex + 1) % options.size();
                    display();
                    break;
                case '\n':
                case '\r':
                    return selectedIndex;
                case 'q':
                case 'Q':
                    return -1; // Quit
            }
        }
    }
};

class GameMenu {
private:
    GameConfig& config;
    
public:
    GameMenu(GameConfig& config) : config(config) {}
    
    void showWelcomeScreen() {
        system("clear");
        
        std::cout << GameColors::MENU_TITLE << R"(
  _____ _   _  _   _  _____ 
 / ____| \ | || \ | ||  __ \
| (___ |  \| ||  \| || |__) |
 \___ \| . ` || . ` ||  ___/ 
 ____) | |\  || |\  || |     
|_____/|_| \_||_| \_||_|     
)" << Colors::RESET << std::endl;
        
        std::cout << GameColors::MENU_OPTION << "Welcome to the Modern Snake Game!" << Colors::RESET << std::endl;
        std::cout << GameColors::MENU_OPTION << "Press ENTER to continue..." << Colors::RESET << std::endl;
        
        while (getchar() != '\n');
    }
    
    int showMainMenu() {
        Menu menu("MAIN MENU");
        menu.addOption("Start Game");
        menu.addOption("Game Modes");
        menu.addOption("Settings");
        menu.addOption("Leaderboard");
        menu.addOption("About");
        menu.addOption("Exit");
        
        menu.display();
        return menu.getSelection();
    }
    
    int showGameModes() {
        Menu menu("GAME MODES");
        menu.addOption("Classic Mode");
        menu.addOption("Speed Mode");
        menu.addOption("Survival Mode");
        menu.addOption("Infinite Mode");
        menu.addOption("Back to Main Menu");
        
        menu.display();
        return menu.getSelection();
    }
    
    int showSettings() {
        Menu menu("SETTINGS");
        menu.addOption("Player Name: " + config.getString("player_name"));
        menu.addOption("Difficulty: " + config.getString("difficulty"));
        menu.addOption(std::string("Sound: ") + (config.getBool("sound_enabled") ? "ON" : "OFF"));
        menu.addOption(std::string("Colors: ") + (config.getBool("colors_enabled") ? "ON" : "OFF"));
        menu.addOption(std::string("Power-ups: ") + (config.getBool("power_ups_enabled") ? "ON" : "OFF"));
        menu.addOption("Reset Settings");
        menu.addOption("Back to Main Menu");
        
        menu.display();
        return menu.getSelection();
    }
    
    void showAbout() {
        system("clear");
        
        std::cout << GameColors::MENU_TITLE << "ABOUT SNAKE GAME" << Colors::RESET << std::endl;
        std::cout << std::string(20, '=') << std::endl << std::endl;
        
        std::cout << GameColors::MENU_OPTION << "Version: 2.0 Modern Edition" << Colors::RESET << std::endl;
        std::cout << GameColors::MENU_OPTION << "Developer: " << config.getString("player_name") << Colors::RESET << std::endl;
        std::cout << GameColors::MENU_OPTION << "Language: C++" << Colors::RESET << std::endl;
        std::cout << GameColors::MENU_OPTION << "Platform: Cross-platform" << Colors::RESET << std::endl << std::endl;
        
        std::cout << GameColors::MENU_OPTION << "Features:" << Colors::RESET << std::endl;
        std::cout << "• Multiple game modes" << std::endl;
        std::cout << "• Power-ups system" << Colors::RESET << std::endl;
        std::cout << "• Colorful graphics" << Colors::RESET << std::endl;
        std::cout << "• Sound effects" << Colors::RESET << std::endl;
        std::cout << "• Leaderboard" << Colors::RESET << std::endl;
        std::cout << "• Configurable settings" << Colors::RESET << std::endl << std::endl;
        
        std::cout << GameColors::MENU_OPTION << "Press ENTER to go back..." << Colors::RESET << std::endl;
        while (getchar() != '\n');
    }
    
    void showGameOver(int score, int highScore, bool newRecord) {
        system("clear");
        
        std::cout << GameColors::GAME_OVER << R"(
  _____ _____ _____ _____    _____ _____ _____ _____ 
 |  _  |  _  |  _  |  _  |  |  _  |  _  |  _  |  _  |
 | |_| | |_| | |_| | |_| |  | |_| | |_| | |_| | |_| |
 |  _  |  _  |  _  |  _  |  |  _  |  _  |  _  |  _  |
 |_| |_|_| |_|_| |_|_| |_|  |_| |_|_| |_|_| |_|_| |_|
)" << Colors::RESET << std::endl;
        
        std::cout << GameColors::SCORE << "Final Score: " << score << Colors::RESET << std::endl;
        std::cout << GameColors::SCORE << "High Score: " << highScore << Colors::RESET << std::endl;
        
        if (newRecord) {
            std::cout << GameColors::SUCCESS << "NEW RECORD!" << Colors::RESET << std::endl;
        }
        
        std::cout << std::endl << GameColors::MENU_OPTION << "Press ENTER to continue..." << Colors::RESET << std::endl;
        while (getchar() != '\n');
    }
};

#endif // MENU_H