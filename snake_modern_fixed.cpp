#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

using namespace std;

// ANSI Colors
#define RESET "\033[0m"
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN "\033[1;36m"
#define WHITE "\033[1;37m"

class ModernSnake {
private:
    static const int WIDTH = 30;
    static const int HEIGHT = 20;
    
    int x, y;
    int fruitX, fruitY;
    int score;
    int highScore;
    vector<int> tailX, tailY;
    int nTail;
    bool gameOver;
    
    enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
    Direction dir;
    
    // Power-ups
    int powerUpX, powerUpY;
    bool powerUpActive;
    char powerUpType;
    bool invincible;
    int invincibleTimer;
    bool doublePoints;
    int doublePointsTimer;
    
    // Terminal setup
    struct termios oldTermios;
    
    void setupTerminal() {
        struct termios newTermios;
        tcgetattr(STDIN_FILENO, &oldTermios);
        newTermios = oldTermios;
        newTermios.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newTermios);
        fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
    }
    
    void restoreTerminal() {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldTermios);
    }
    
    void spawnFruit() {
        bool validPos = false;
        while (!validPos) {
            fruitX = rand() % WIDTH;
            fruitY = rand() % HEIGHT;
            
            validPos = true;
            if (fruitX == x && fruitY == y) validPos = false;
            
            for (int i = 0; i < nTail; i++) {
                if (tailX[i] == fruitX && tailY[i] == fruitY) {
                    validPos = false;
                    break;
                }
            }
        }
    }
    
    void spawnPowerUp() {
        if (rand() % 100 < 20) {  // 20% chance
            powerUpX = rand() % WIDTH;
            powerUpY = rand() % HEIGHT;
            char types[] = {'S', 'D', 'I', 'G', 'R'};
            powerUpType = types[rand() % 5];
            powerUpActive = true;
        }
    }
    
    void applyPowerUp() {
        switch (powerUpType) {
            case 'S': // Speed boost (handled in main loop)
                break;
            case 'D': // Double points
                doublePoints = true;
                doublePointsTimer = 50;
                break;
            case 'I': // Invincible
                invincible = true;
                invincibleTimer = 30;
                break;
            case 'G': // Growth
                nTail += 3;
                break;
            case 'R': // Shrink
                if (nTail > 3) nTail -= 3;
                break;
        }
        powerUpActive = false;
    }
    
public:
    ModernSnake() {
        srand(time(NULL));
        setup();
        highScore = 0;
    }
    
    void setup() {
        x = WIDTH / 2;
        y = HEIGHT / 2;
        score = 0;
        nTail = 0;
        dir = STOP;
        gameOver = false;
        invincible = false;
        doublePoints = false;
        powerUpActive = false;
        invincibleTimer = 0;
        doublePointsTimer = 0;
        
        tailX.clear();
        tailY.clear();
        tailX.resize(200, -1);
        tailY.resize(200, -1);
        
        spawnFruit();
    }
    
    void draw() {
        system("clear");
        
        // Title
        cout << CYAN << "=== MODERN SNAKE GAME ===" << RESET << endl;
        
        // Top wall
        cout << WHITE;
        for (int i = 0; i < WIDTH + 2; i++) cout << "#";
        cout << RESET << endl;
        
        // Game area
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                if (j == 0) cout << WHITE << "#" << RESET;
                
                if (i == y && j == x) {
                    cout << GREEN << "O" << RESET;  // Snake head
                }
                else if (i == fruitY && j == fruitX) {
                    cout << RED << "X" << RESET;  // Fruit
                }
                else if (powerUpActive && i == powerUpY && j == powerUpX) {
                    cout << YELLOW << powerUpType << RESET;  // Power-up
                }
                else {
                    bool printed = false;
                    for (int k = 0; k < nTail; k++) {
                        if (tailX[k] == j && tailY[k] == i) {
                            cout << GREEN << "o" << RESET;  // Snake body
                            printed = true;
                            break;
                        }
                    }
                    if (!printed) cout << " ";
                }
                
                if (j == WIDTH - 1) cout << WHITE << "#" << RESET;
            }
            cout << endl;
        }
        
        // Bottom wall
        cout << WHITE;
        for (int i = 0; i < WIDTH + 2; i++) cout << "#";
        cout << RESET << endl;
        
        // Score and status
        cout << CYAN << "Score: " << score << RESET;
        cout << "  " << MAGENTA << "High Score: " << highScore << RESET << endl;
        
        if (invincible) {
            cout << YELLOW << "INVINCIBLE!" << RESET << " ";
        }
        if (doublePoints) {
            cout << YELLOW << "DOUBLE POINTS!" << RESET << " ";
        }
        cout << endl;
        
        cout << "Controls: WASD=Move  P=Pause  X=Quit" << endl;
        cout << "Power-ups: " << YELLOW << "S" << RESET << "=Speed " 
             << YELLOW << "D" << RESET << "=2xPoints "
             << YELLOW << "I" << RESET << "=Invincible "
             << YELLOW << "G" << RESET << "=Grow "
             << YELLOW << "R" << RESET << "=Shrink" << endl;
    }
    
    void input() {
        char ch;
        if (read(STDIN_FILENO, &ch, 1) == 1) {
            switch (ch) {
                case 'a': case 'A':
                    if (dir != RIGHT) dir = LEFT;
                    break;
                case 'd': case 'D':
                    if (dir != LEFT) dir = RIGHT;
                    break;
                case 'w': case 'W':
                    if (dir != DOWN) dir = UP;
                    break;
                case 's': case 'S':
                    if (dir != UP) dir = DOWN;
                    break;
                case 'x': case 'X':
                    gameOver = true;
                    break;
                case 'p': case 'P':
                    cout << "\nPAUSED - Press any key to continue...";
                    cin.get();
                    break;
            }
        }
    }
    
    void logic() {
        // Update timers
        if (invincibleTimer > 0) {
            invincibleTimer--;
            if (invincibleTimer == 0) invincible = false;
        }
        if (doublePointsTimer > 0) {
            doublePointsTimer--;
            if (doublePointsTimer == 0) doublePoints = false;
        }
        
        // Move tail
        int prevX = tailX[0];
        int prevY = tailY[0];
        tailX[0] = x;
        tailY[0] = y;
        
        for (int i = 1; i < nTail; i++) {
            int prev2X = tailX[i];
            int prev2Y = tailY[i];
            tailX[i] = prevX;
            tailY[i] = prevY;
            prevX = prev2X;
            prevY = prev2Y;
        }
        
        // Move head
        switch (dir) {
            case LEFT:  x--; break;
            case RIGHT: x++; break;
            case UP:    y--; break;
            case DOWN:  y++; break;
            default: break;
        }
        
        // Wall collision
        if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
            if (!invincible) {
                gameOver = true;
            } else {
                // Wrap around when invincible
                if (x < 0) x = WIDTH - 1;
                if (x >= WIDTH) x = 0;
                if (y < 0) y = HEIGHT - 1;
                if (y >= HEIGHT) y = 0;
            }
        }
        
        // Self collision
        if (!invincible) {
            for (int i = 0; i < nTail; i++) {
                if (tailX[i] == x && tailY[i] == y) {
                    gameOver = true;
                    break;
                }
            }
        }
        
        // Fruit collision
        if (x == fruitX && y == fruitY) {
            int points = 10;
            if (doublePoints) points *= 2;
            
            score += points;
            nTail++;
            
            spawnFruit();
            spawnPowerUp();
        }
        
        // Power-up collision
        if (powerUpActive && x == powerUpX && y == powerUpY) {
            applyPowerUp();
        }
    }
    
    void run() {
        setupTerminal();
        
        cout << CYAN << "\n=== MODERN SNAKE GAME ===\n" << RESET;
        cout << "Press any key to start...\n";
        cin.get();
        
        while (!gameOver) {
            draw();
            input();
            logic();
            
            int sleepTime = 100000 - (score * 500);
            if (sleepTime < 30000) sleepTime = 30000;
            usleep(sleepTime);
        }
        
        restoreTerminal();
        
        // Game over
        system("clear");
        cout << RED << "\n=== GAME OVER ===\n" << RESET;
        cout << CYAN << "Final Score: " << score << RESET << endl;
        
        if (score > highScore) {
            highScore = score;
            cout << YELLOW << "NEW HIGH SCORE!\n" << RESET;
        }
        
        cout << MAGENTA << "High Score: " << highScore << RESET << endl;
        cout << "\nPress Enter to continue...";
        cin.get();
    }
    
    bool isGameOver() { return gameOver; }
    int getScore() { return score; }
};

int main() {
    char playAgain;
    
    do {
        ModernSnake game;
        game.run();
        
        cout << "\nPlay again? (y/n): ";
        cin >> playAgain;
        cin.ignore();
        
    } while (playAgain == 'y' || playAgain == 'Y');
    
    cout << "Thanks for playing!\n";
    return 0;
}
