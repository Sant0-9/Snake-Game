#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <cstdlib>
#include <fstream> 


using namespace std;

void Draw();
void Input();
void Logic();

bool gameover = false;
const int width = 30;
const int height = 30;

int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;

enum Direction
{
    STOP = 0,
    LEFT,
    RIGHT,
    UP,
    DOWN
};
Direction dir;

void setup()
{
    for (int i = 0; i < 100; i++)
    {
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

void Draw()
{
    system("cls");
    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (j == 0)
                cout << "#";
            if (i == y && j == x)
                cout << "O";
            else if (i == fruitY && j == fruitX)
                cout << "X";
            else
            {
                bool print = false;
                for (int k = 0; k < nTail; k++)
                {
                    if (tailX[k] == j && tailY[k] == i)
                    {
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

    for (int i = 0; i < width + 2; i++)
    {
        cout << "#";
    }
    cout << endl;
    cout << "Score: " << score << endl;
}

void Input()
{
    if (_kbhit())
    {
        switch (_getch())
        {
        case 'a':
            if (dir != RIGHT)
                dir = LEFT;
            break;
        case 'd':
            if (dir != LEFT)
                dir = RIGHT;
            break;
        case 'w':
            if (dir != DOWN)
                dir = UP;
            break;
        case 's':
            if (dir != UP)
                dir = DOWN;
            break;

        case 'x':
            gameover = true;
            break;
        }
    }
}

void Logic()
{
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;

    tailX[0] = x;
    tailY[0] = y;

    for (int i = 1; i < nTail; i++)
    {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir)
    {
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
    if (x == fruitX && y == fruitY)
    {
        score += 10;
        nTail++;

        // Write the current score to the file
        ofstream file("score.txt");
        if (file.is_open())
        {
            file << "Current Score: " << score << endl;
            file.close();
        }

        // Find a new position for the fruit
        bool validPosition = false;
        while (!validPosition)
        {
            fruitX = rand() % width;
            fruitY = rand() % height;

            validPosition = true;
            if (fruitX == x && fruitY == y)
                validPosition = false;
            for (int i = 0; i < nTail; i++)
            {
                if (tailX[i] == fruitX && tailY[i] == fruitY)
                {
                    validPosition = false;
                    break;
                }
            }
        }
    }
}


int main()
{
    // Read max score from file
    int maxScore = 0;
    ifstream readFile("score.txt");
    if (readFile.is_open())
    {
        string line;
        while (getline(readFile, line))
        {
            if (line.find("Highest Score:") != string::npos)
            {
                maxScore = stoi(line.substr(line.find(":") + 1));
            }
        }
        readFile.close();
    }

    setup();
    while (!gameover)
    {
        Draw();
        Input();
        Logic();
        Sleep(max(10, 100 - score));
    }

    // Update max score if the current score is higher
    if (score > maxScore)
        maxScore = score;

    // Write both current and max scores to file
    ofstream file("score.txt");
    if (file.is_open())
    {
        file << "Current Score: " << score << endl;
        file << "Max Score: " << maxScore << endl;
        file.close();
    }

    cout << "Game Over! Final Score: " << score << endl;
    cout << "Highest Score: " << maxScore << endl;
    return 0;
}
