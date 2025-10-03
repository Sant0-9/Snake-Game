#include <iostream>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <cstring>
#include <chrono>
#include <thread>
#include <random>

#include "colors.h"
#include "config.h"
#include "powerups.h"
#include "menu.h"
#include "sound.h"
#include "leaderboard.h"

using namespace std;

class ModernSnakeGame {
private:
    bool gameover;
    int width, height;
    int x, y, fruitX, fruitY, score;
    vector<int> tailX, tailY;
    int nTail;
    GameMode gameMode;
    Difficulty difficulty;
    
    enum Direction {
        STOP = 0,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };
    Direction dir;
    
    // Modern features
    GameConfig config;
    PowerUpManager powerUpManager;
    SoundManager soundManager;
    Leaderboard leaderboard;
    
    // Game state
    bool paused;
    bool invincible;
    bool doublePoints;
    chrono::steady_clock::time_point invincibleEnd;
    chrono::steady_clock::time_point doublePointsEnd;
    chrono::steady_clock::time_point speedBoostEnd;
    double speedMultiplier;
    
    // Terminal settings
    struct termios original_termios;
    bool terminal_modified;
    
public:
    ModernSnakeGame() : gameover(false), width(30), height(30), score(0), nTail(0), dir(STOP), 
                       terminal_modified(false), paused(false), invincible(false), doublePoints(false),
                       speedMultiplier(1.0), powerUpManager(30, 30), soundManager(config) {
        tailX.resize(100, -1);
        tailY.resize(100, -1);
        
        // Load settings
        width = config.getInt("board_width");
        height = config.getInt("board_height");
        speedMultiplier = config.getDouble("speed_multiplier");
        
        // Initialize power-up manager with correct dimensions
        powerUpManager = PowerUpManager(width, height);
    }
    
    ~ModernSnakeGame() {
        restoreTerminal();
    }
    
    void setupTerminal() {
        tcgetattr(STDIN_FILENO, &original_termios);
        struct termios new_termios = original_termios;
        new_termios.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
        terminal_modified = true;
    }
    
    void restoreTerminal() {
        if (terminal_modified) {
            tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
            terminal_modified = false;
        }
    }
    
    bool kbhit() {
        int ch = getchar();
        if (ch != EOF) {
            ungetc(ch, stdin);
            return true;
        }
        return false;
    }
    
    char getch() {
        return getchar();
    }
    
    void setup() {
        for (int i = 0; i < 100; i++) {
            tailX[i] = -1;
            tailY[i] = -1;
        }
        gameover = false;
        paused = false;
        invincible = false;
        doublePoints = false;
        dir = STOP;
        x = width / 2;
        y = height / 2;
        fruitX = rand() % width;
        fruitY = rand() % height;
        score = 0;
        nTail = 0;
        speedMultiplier = config.getDouble("speed_multiplier");
        
        // Clear power-ups
        powerUpManager.clear();
    }
    
    void Draw() {
        if (system("clear") != 0) {
            cout << "\033[2J\033[1;1H";
        }
        
        // Draw top border
        for (int i = 0; i < width + 2; i++)
            cout << GameColors::WALL << "#" << Colors::RESET;
        cout << endl;
        
        // Draw game area
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (j == 0)
                    cout << GameColors::WALL << "#" << Colors::RESET;
                
                if (i == y && j == x) {
                    cout << GameColors::SNAKE_HEAD << "O" << Colors::RESET;
                } else if (i == fruitY && j == fruitX) {
                    cout << GameColors::FRUIT << "X" << Colors::RESET;
                } else {
                    bool print = false;
                    
                    // Draw snake body
                    for (int k = 0; k < nTail; k++) {
                        if (tailX[k] == j && tailY[k] == i) {
                            cout << GameColors::SNAKE_BODY << "o" << Colors::RESET;
                            print = true;
                            break;
                        }
                    }
                    
                    // Draw power-ups
                    if (!print) {
                        for (const auto& powerUp : powerUpManager.getPowerUps()) {
                            if (powerUp.x == j && powerUp.y == i && powerUp.active) {
                                cout << powerUp.getColor() << powerUp.getSymbol() << Colors::RESET;
                                print = true;
                                break;
                            }
                        }
                    }
                    
                    if (!print)
                        cout << " ";
                }
                
                if (j == width - 1)
                    cout << GameColors::WALL << "#" << Colors::RESET;
            }
            cout << "\n";
        }
        
        // Draw bottom border
        for (int i = 0; i < width + 2; i++)
            cout << GameColors::WALL << "#" << Colors::RESET;
        cout << endl;
        
        // Draw game info
        cout << GameColors::SCORE << "Score: " << score << Colors::RESET << endl;
        cout << GameColors::SCORE << "High Score: " << leaderboard.getHighScore() << Colors::RESET << endl;
        
        if (invincible) {
            cout << GameColors::SUCCESS << "INVINCIBLE!" << Colors::RESET << endl;
        }
        if (doublePoints) {
            cout << GameColors::SUCCESS << "DOUBLE POINTS!" << Colors::RESET << endl;
        }
        if (speedMultiplier > 1.0) {
            cout << GameColors::SUCCESS << "SPEED BOOST!" << Colors::RESET << endl;
        }
        
        cout << GameColors::MENU_OPTION << "Controls: WASD to move, P to pause, X to quit" << Colors::RESET << endl;
    }
    
    void Input() {
        if (kbhit()) {
            char key = getch();
            switch (key) {
                case 'a':
                case 'A':
                    if (dir != RIGHT)
                        dir = LEFT;
                    break;
                case 'd':
                case 'D':
                    if (dir != LEFT)
                        dir = RIGHT;
                    break;
                case 'w':
                case 'W':
                    if (dir != DOWN)
                        dir = UP;
                    break;
                case 's':
                case 'S':
                    if (dir != UP)
                        dir = DOWN;
                    break;
                case 'p':
                case 'P':
                    pause();
                    break;
                case 'x':
                case 'X':
                    gameover = true;
                    break;
            }
        }
    }
    
    void pause() {
        paused = !paused;
        if (paused) {
            cout << "\n" << GameColors::MENU_OPTION << "Game Paused! Press 'P' to continue or 'X' to quit..." << Colors::RESET << endl;
            while (paused && !gameover) {
                if (kbhit()) {
                    char key = getch();
                    if (key == 'p' || key == 'P') {
                        paused = false;
                    } else if (key == 'x' || key == 'X') {
                        gameover = true;
                    }
                }
                usleep(100000);
            }
        }
    }
    
    void Logic() {
        if (paused) return;
        
        // Update power-up effects
        updatePowerUpEffects();
        
        // Move snake
        int prevX = tailX[0];
        int prevY = tailY[0];
        int prev2X, prev2Y;
        
        tailX[0] = x;
        tailY[0] = y;
        
        for (int i = 1; i < nTail; i++) {
            prev2X = tailX[i];
            prev2Y = tailY[i];
            tailX[i] = prevX;
            tailY[i] = prevY;
            prevX = prev2X;
            prevY = prev2Y;
        }
        
        switch (dir) {
            case LEFT: x--; break;
            case RIGHT: x++; break;
            case UP: y--; break;
            case DOWN: y++; break;
            default: break;
        }
        
        // Check wall collision
        if (x >= width || x < 0 || y >= height || y < 0) {
            if (!invincible) {
                gameover = true;
            } else {
                // Wrap around if invincible
                if (x >= width) x = 0;
                if (x < 0) x = width - 1;
                if (y >= height) y = 0;
                if (y < 0) y = height - 1;
            }
        }
        
        // Check tail collision
        if (!invincible) {
            for (int i = 0; i < nTail; i++) {
                if (tailX[i] == x && tailY[i] == y) {
                    gameover = true;
                    break;
                }
            }
        }
        
        // Check fruit collision
        if (x == fruitX && y == fruitY) {
            int points = 10;
            if (doublePoints) points *= 2;
            
            score += points;
            nTail++;
            soundManager.playEatSound();
            
            // Try to spawn power-up
            if (config.getBool("power_ups_enabled")) {
                powerUpManager.trySpawnPowerUp(fruitX, fruitY, tailX, tailY, nTail);
            }
            
            // Find new fruit position
            bool validPosition = false;
            while (!validPosition) {
                fruitX = rand() % width;
                fruitY = rand() % height;
                
                validPosition = true;
                if (fruitX == x && fruitY == y)
                    validPosition = false;
                for (int i = 0; i < nTail; i++) {
                    if (tailX[i] == fruitX && tailY[i] == fruitY) {
                        validPosition = false;
                        break;
                    }
                }
            }
        }
        
        // Check power-up collision
        PowerUp* powerUp = powerUpManager.checkCollision(x, y);
        if (powerUp) {
            applyPowerUp(powerUp);
            soundManager.playPowerUpSound();
        }
        
        // Update power-ups
        powerUpManager.update();
    }
    
    void updatePowerUpEffects() {
        auto now = chrono::steady_clock::now();
        
        if (invincible && now > invincibleEnd) {
            invincible = false;
        }
        
        if (doublePoints && now > doublePointsEnd) {
            doublePoints = false;
        }
        
        if (speedMultiplier > 1.0 && now > speedBoostEnd) {
            speedMultiplier = config.getDouble("speed_multiplier");
        }
    }
    
    void applyPowerUp(PowerUp* powerUp) {
        auto now = chrono::steady_clock::now();
        
        switch (powerUp->type) {
            case PowerUpType::SPEED_BOOST:
                speedMultiplier = 2.0;
                speedBoostEnd = now + chrono::seconds(5);
                break;
            case PowerUpType::SLOW_DOWN:
                speedMultiplier = 0.5;
                speedBoostEnd = now + chrono::seconds(5);
                break;
            case PowerUpType::DOUBLE_POINTS:
                doublePoints = true;
                doublePointsEnd = now + chrono::seconds(10);
                break;
            case PowerUpType::INVINCIBLE:
                invincible = true;
                invincibleEnd = now + chrono::seconds(3);
                break;
            case PowerUpType::SHRINK:
                if (nTail >= 3) {
                    nTail -= 3;
                } else {
                    nTail = 0;
                }
                break;
            case PowerUpType::GROWTH_BOOST:
                nTail += 5;
                break;
            case PowerUpType::TELEPORT:
                x = rand() % width;
                y = rand() % height;
                break;
        }
    }
    
    void run() {
        setupTerminal();
        setup();
        
        while (!gameover) {
            Draw();
            Input();
            Logic();
            
            // Adjust sleep based on speed multiplier
            int baseSleep = max(10000, 100000 - score * 1000);
            int adjustedSleep = static_cast<int>(baseSleep / speedMultiplier);
            usleep(adjustedSleep);
        }
        
        soundManager.playGameOverSound();
        
        // Save score to leaderboard
        string gameModeStr = "Classic"; // You can expand this based on actual game mode
        string difficultyStr = config.getString("difficulty");
        leaderboard.addScore(config.getString("player_name"), score, gameModeStr, difficultyStr);
        
        restoreTerminal();
    }
    
    void showGameOver() {
        bool newRecord = leaderboard.isNewRecord(score);
        GameMenu menu(config);
        menu.showGameOver(score, leaderboard.getHighScore(), newRecord);
    }
};

int main() {
    GameConfig config;
    GameMenu menu(config);
    Leaderboard leaderboard;
    
    while (true) {
        menu.showWelcomeScreen();
        
        int choice = menu.showMainMenu();
        
        switch (choice) {
            case 0: { // Start Game
                ModernSnakeGame game;
                game.run();
                game.showGameOver();
                break;
            }
            case 1: { // Game Modes
                int modeChoice = menu.showGameModes();
                if (modeChoice >= 0 && modeChoice < 4) {
                    // Set game mode in config
                    string modes[] = {"classic", "speed", "survival", "infinite"};
                    config.setString("game_mode", modes[modeChoice]);
                    config.saveToFile();
                }
                break;
            }
            case 2: { // Settings
                menu.showSettings();
                // Handle settings changes here
                break;
            }
            case 3: { // Leaderboard
                leaderboard.displayLeaderboard();
                break;
            }
            case 4: { // About
                menu.showAbout();
                break;
            }
            case 5: // Exit
            case -1: // Quit
                return 0;
        }
    }
    
    return 0;
}