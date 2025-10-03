#include <iostream>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <cstring>

using namespace std;

class SnakeGame {
private:
    bool gameover;
    const int width;
    const int height;
    int x, y, fruitX, fruitY, score;
    vector<int> tailX, tailY;
    int nTail;
    
    enum Direction {
        STOP = 0,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };
    Direction dir;
    
    // Terminal settings for non-blocking input
    struct termios original_termios;
    bool terminal_modified;
    
public:
    SnakeGame() : gameover(false), width(30), height(30), score(0), nTail(0), dir(STOP), terminal_modified(false) {
        tailX.resize(100, -1);
        tailY.resize(100, -1);
    }
    
    ~SnakeGame() {
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
        dir = STOP;
        x = width / 2;
        y = height / 2;
        fruitX = rand() % width;
        fruitY = rand() % height;
        score = 0;
        nTail = 0;
    }
    
    void Draw() {
        if (system("clear") != 0) {
            // Fallback if clear command fails
            cout << "\033[2J\033[1;1H"; // ANSI escape sequence
        }
        for (int i = 0; i < width + 2; i++)
            cout << "#";
        cout << endl;
        
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (j == 0)
                    cout << "#";
                if (i == y && j == x)
                    cout << "O";
                else if (i == fruitY && j == fruitX)
                    cout << "X";
                else {
                    bool print = false;
                    for (int k = 0; k < nTail; k++) {
                        if (tailX[k] == j && tailY[k] == i) {
                            cout << "o";
                            print = true;
                        }
                    }
                    if (!print)
                        cout << " ";
                }
                if (j == width - 1)
                    cout << "#";
            }
            cout << "\n";
        }
        
        for (int i = 0; i < width + 2; i++) {
            cout << "#";
        }
        cout << endl;
        cout << "Score: " << score << endl;
        cout << "Controls: WASD to move, 'x' to quit, 'p' to pause" << endl;
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
            case 'x':
            case 'X':
                gameover = true;
                break;
            case 'p':
            case 'P':
                pause();
                break;
            case 'r':
            case 'R':
                setup();
                break;
            }
        }
    }
    
    void pause() {
        cout << "\nGame Paused! Press 'p' to continue or 'x' to quit..." << endl;
        while (true) {
            if (kbhit()) {
                char key = getch();
                if (key == 'p' || key == 'P') {
                    break;
                } else if (key == 'x' || key == 'X') {
                    gameover = true;
                    break;
                }
            }
            usleep(100000); // Sleep for 100ms
        }
    }
    
    void Logic() {
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
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        default:
            break;
        }
        
        // End the game if the snake hits the wall
        if (x >= width || x < 0 || y >= height || y < 0)
            gameover = true;
        
        // Check for collision with tail
        for (int i = 0; i < nTail; i++)
            if (tailX[i] == x && tailY[i] == y)
                gameover = true;
        
        // Check for fruit collision
        if (x == fruitX && y == fruitY) {
            score += 10;
            nTail++;
            
            // Write the current score to the file
            ofstream file("score.txt");
            if (file.is_open()) {
                file << "Current Score: " << score << endl;
                file.close();
            }
            
            // Find a new position for the fruit
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
    }
    
    void run() {
        // Read max score from file
        int maxScore = 0;
        ifstream readFile("score.txt");
        if (readFile.is_open()) {
            string line;
            while (getline(readFile, line)) {
                if (line.find("Max Score:") != string::npos) {
                    maxScore = stoi(line.substr(line.find(":") + 1));
                }
            }
            readFile.close();
        }
        
        setupTerminal();
        setup();
        
        while (!gameover) {
            Draw();
            Input();
            Logic();
            usleep(max(10000, 100000 - score * 1000)); // Sleep in microseconds
        }
        
        // Update max score if the current score is higher
        if (score > maxScore)
            maxScore = score;
        
        // Write both current and max scores to file
        ofstream file("score.txt");
        if (file.is_open()) {
            file << "Current Score: " << score << endl;
            file << "Max Score: " << maxScore << endl;
            file.close();
        }
        
        restoreTerminal();
        cout << "Game Over! Final Score: " << score << endl;
        cout << "Highest Score: " << maxScore << endl;
        cout << "Press Enter to exit..." << endl;
        cin.get();
    }
};

int main() {
    SnakeGame game;
    game.run();
    return 0;
}