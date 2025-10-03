#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>
#include <sstream>

using namespace std;

// Game constants
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int GRID_SIZE = 20;
const int GRID_WIDTH = WINDOW_WIDTH / GRID_SIZE;
const int GRID_HEIGHT = WINDOW_HEIGHT / GRID_SIZE;
const int FPS = 10;

// Colors
SDL_Color COLOR_BACKGROUND = {20, 20, 40, 255};
SDL_Color COLOR_SNAKE_HEAD = {50, 255, 50, 255};
SDL_Color COLOR_SNAKE_BODY = {30, 200, 30, 255};
SDL_Color COLOR_FRUIT = {255, 50, 50, 255};
SDL_Color COLOR_POWERUP = {255, 200, 50, 255};
SDL_Color COLOR_TEXT = {255, 255, 255, 255};
SDL_Color COLOR_GRID = {40, 40, 60, 255};

enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
enum PowerUpType { SPEED_BOOST, DOUBLE_POINTS, INVINCIBLE, GROWTH, SHRINK };

struct PowerUp {
    int x, y;
    PowerUpType type;
    Uint32 spawnTime;
    bool active;
};

class SnakeGameGUI {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    TTF_Font* titleFont;
    
    int snakeX, snakeY;
    int fruitX, fruitY;
    vector<int> tailX, tailY;
    int nTail;
    int score;
    int highScore;
    Direction dir;
    bool gameOver;
    bool paused;
    
    PowerUp powerUp;
    bool invincible;
    bool doublePoints;
    Uint32 invincibleEnd;
    Uint32 doublePointsEnd;
    
    int gameSpeed;
    Uint32 lastMoveTime;
    
    bool init() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            return false;
        }
        
        if (TTF_Init() < 0) {
            return false;
        }
        
        window = SDL_CreateWindow(
            "Modern Snake Game - GUI Edition",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            SDL_WINDOW_SHOWN
        );
        
        if (!window) {
            return false;
        }
        
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (!renderer) {
            return false;
        }
        
        // Try to load font (use system fonts as fallback)
        font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 18);
        if (!font) {
            font = TTF_OpenFont("/usr/share/fonts/TTF/DejaVuSans.ttf", 18);
        }
        
        titleFont = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", 36);
        if (!titleFont) {
            titleFont = TTF_OpenFont("/usr/share/fonts/TTF/DejaVuSans-Bold.ttf", 36);
        }
        
        return true;
    }
    
    void spawnFruit() {
        bool validPos = false;
        while (!validPos) {
            fruitX = rand() % GRID_WIDTH;
            fruitY = rand() % GRID_HEIGHT;
            
            validPos = true;
            if (fruitX == snakeX && fruitY == snakeY) validPos = false;
            
            for (int i = 0; i < nTail; i++) {
                if (tailX[i] == fruitX && tailY[i] == fruitY) {
                    validPos = false;
                    break;
                }
            }
        }
    }
    
    void spawnPowerUp() {
        if (rand() % 100 < 25) {
            powerUp.x = rand() % GRID_WIDTH;
            powerUp.y = rand() % GRID_HEIGHT;
            powerUp.type = (PowerUpType)(rand() % 5);
            powerUp.spawnTime = SDL_GetTicks();
            powerUp.active = true;
        }
    }
    
    void applyPowerUp() {
        Uint32 currentTime = SDL_GetTicks();
        
        switch (powerUp.type) {
            case SPEED_BOOST:
                gameSpeed = 5;
                break;
            case DOUBLE_POINTS:
                doublePoints = true;
                doublePointsEnd = currentTime + 5000;
                break;
            case INVINCIBLE:
                invincible = true;
                invincibleEnd = currentTime + 3000;
                break;
            case GROWTH:
                nTail += 3;
                break;
            case SHRINK:
                if (nTail > 3) nTail -= 3;
                break;
        }
        powerUp.active = false;
    }
    
    void drawRect(int gridX, int gridY, SDL_Color color) {
        SDL_Rect rect = {
            gridX * GRID_SIZE,
            gridY * GRID_SIZE,
            GRID_SIZE - 1,
            GRID_SIZE - 1
        };
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer, &rect);
    }
    
    void drawCircle(int gridX, int gridY, SDL_Color color) {
        int centerX = gridX * GRID_SIZE + GRID_SIZE / 2;
        int centerY = gridY * GRID_SIZE + GRID_SIZE / 2;
        int radius = GRID_SIZE / 2 - 2;
        
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        for (int w = 0; w < radius * 2; w++) {
            for (int h = 0; h < radius * 2; h++) {
                int dx = radius - w;
                int dy = radius - h;
                if ((dx*dx + dy*dy) <= (radius * radius)) {
                    SDL_RenderDrawPoint(renderer, centerX + dx, centerY + dy);
                }
            }
        }
    }
    
    void drawText(const string& text, int x, int y, SDL_Color color, TTF_Font* useFont = nullptr) {
        if (!useFont) useFont = font;
        if (!useFont) return;
        
        SDL_Surface* surface = TTF_RenderText_Solid(useFont, text.c_str(), color);
        if (!surface) return;
        
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture) {
            SDL_FreeSurface(surface);
            return;
        }
        
        SDL_Rect destRect = {x, y, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, nullptr, &destRect);
        
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }
    
    void drawGrid() {
        SDL_SetRenderDrawColor(renderer, COLOR_GRID.r, COLOR_GRID.g, COLOR_GRID.b, COLOR_GRID.a);
        for (int x = 0; x < WINDOW_WIDTH; x += GRID_SIZE) {
            SDL_RenderDrawLine(renderer, x, 0, x, WINDOW_HEIGHT);
        }
        for (int y = 0; y < WINDOW_HEIGHT; y += GRID_SIZE) {
            SDL_RenderDrawLine(renderer, 0, y, WINDOW_WIDTH, y);
        }
    }
    
    void render() {
        // Clear screen
        SDL_SetRenderDrawColor(renderer, COLOR_BACKGROUND.r, COLOR_BACKGROUND.g, COLOR_BACKGROUND.b, 255);
        SDL_RenderClear(renderer);
        
        // Draw grid
        drawGrid();
        
        // Draw snake body
        for (int i = 0; i < nTail; i++) {
            drawRect(tailX[i], tailY[i], COLOR_SNAKE_BODY);
        }
        
        // Draw snake head (with glow effect if invincible)
        if (invincible) {
            SDL_Color glowColor = {100, 255, 255, 255};
            drawCircle(snakeX, snakeY, glowColor);
        }
        drawCircle(snakeX, snakeY, COLOR_SNAKE_HEAD);
        
        // Draw fruit
        drawCircle(fruitX, fruitY, COLOR_FRUIT);
        
        // Draw power-up
        if (powerUp.active) {
            drawCircle(powerUp.x, powerUp.y, COLOR_POWERUP);
        }
        
        // Draw UI
        stringstream ss;
        ss << "Score: " << score;
        drawText(ss.str(), 10, 10, COLOR_TEXT);
        
        ss.str("");
        ss << "High Score: " << highScore;
        drawText(ss.str(), 10, 35, COLOR_TEXT);
        
        if (invincible) {
            drawText("INVINCIBLE!", 10, 60, {255, 255, 0, 255});
        }
        if (doublePoints) {
            drawText("2X POINTS!", 10, 85, {255, 200, 0, 255});
        }
        
        if (paused) {
            drawText("PAUSED", WINDOW_WIDTH/2 - 50, WINDOW_HEIGHT/2 - 20, COLOR_TEXT, titleFont);
            drawText("Press SPACE to continue", WINDOW_WIDTH/2 - 120, WINDOW_HEIGHT/2 + 30, COLOR_TEXT);
        }
        
        if (gameOver) {
            drawText("GAME OVER!", WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 - 50, {255, 50, 50, 255}, titleFont);
            
            ss.str("");
            ss << "Final Score: " << score;
            drawText(ss.str(), WINDOW_WIDTH/2 - 80, WINDOW_HEIGHT/2 + 10, COLOR_TEXT);
            
            drawText("Press R to Restart or ESC to Quit", WINDOW_WIDTH/2 - 150, WINDOW_HEIGHT/2 + 50, COLOR_TEXT);
        }
        
        SDL_RenderPresent(renderer);
    }
    
    void handleInput(SDL_Event& event) {
        if (event.type == SDL_QUIT) {
            gameOver = true;
            return;
        }
        
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_UP:
                case SDLK_w:
                    if (dir != DOWN && !paused) dir = UP;
                    break;
                case SDLK_DOWN:
                case SDLK_s:
                    if (dir != UP && !paused) dir = DOWN;
                    break;
                case SDLK_LEFT:
                case SDLK_a:
                    if (dir != RIGHT && !paused) dir = LEFT;
                    break;
                case SDLK_RIGHT:
                case SDLK_d:
                    if (dir != LEFT && !paused) dir = RIGHT;
                    break;
                case SDLK_SPACE:
                    if (!gameOver) paused = !paused;
                    break;
                case SDLK_r:
                    if (gameOver) reset();
                    break;
                case SDLK_ESCAPE:
                    gameOver = true;
                    break;
            }
        }
    }
    
    void logic() {
        if (paused || gameOver) return;
        
        Uint32 currentTime = SDL_GetTicks();
        
        // Update power-up timers
        if (invincible && currentTime > invincibleEnd) {
            invincible = false;
        }
        if (doublePoints && currentTime > doublePointsEnd) {
            doublePoints = false;
            gameSpeed = 10;
        }
        
        // Check if it's time to move
        int moveDelay = 1000 / gameSpeed;
        if (currentTime - lastMoveTime < moveDelay) {
            return;
        }
        lastMoveTime = currentTime;
        
        // Move tail
        int prevX = tailX[0];
        int prevY = tailY[0];
        tailX[0] = snakeX;
        tailY[0] = snakeY;
        
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
            case LEFT:  snakeX--; break;
            case RIGHT: snakeX++; break;
            case UP:    snakeY--; break;
            case DOWN:  snakeY++; break;
            default: break;
        }
        
        // Wall collision
        if (snakeX < 0 || snakeX >= GRID_WIDTH || snakeY < 0 || snakeY >= GRID_HEIGHT) {
            if (!invincible) {
                gameOver = true;
            } else {
                // Wrap around
                if (snakeX < 0) snakeX = GRID_WIDTH - 1;
                if (snakeX >= GRID_WIDTH) snakeX = 0;
                if (snakeY < 0) snakeY = GRID_HEIGHT - 1;
                if (snakeY >= GRID_HEIGHT) snakeY = 0;
            }
        }
        
        // Self collision
        if (!invincible) {
            for (int i = 0; i < nTail; i++) {
                if (tailX[i] == snakeX && tailY[i] == snakeY) {
                    gameOver = true;
                    break;
                }
            }
        }
        
        // Fruit collision
        if (snakeX == fruitX && snakeY == fruitY) {
            int points = 10;
            if (doublePoints) points *= 2;
            
            score += points;
            if (score > highScore) highScore = score;
            
            nTail++;
            spawnFruit();
            spawnPowerUp();
            
            // Increase speed
            if (gameSpeed < 20) gameSpeed++;
        }
        
        // Power-up collision
        if (powerUp.active && snakeX == powerUp.x && snakeY == powerUp.y) {
            applyPowerUp();
        }
        
        // Expire power-up
        if (powerUp.active && currentTime - powerUp.spawnTime > 10000) {
            powerUp.active = false;
        }
    }
    
public:
    SnakeGameGUI() {
        window = nullptr;
        renderer = nullptr;
        font = nullptr;
        titleFont = nullptr;
        highScore = 0;
        srand(time(nullptr));
    }
    
    ~SnakeGameGUI() {
        if (font) TTF_CloseFont(font);
        if (titleFont) TTF_CloseFont(titleFont);
        if (renderer) SDL_DestroyRenderer(renderer);
        if (window) SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
    }
    
    void reset() {
        snakeX = GRID_WIDTH / 2;
        snakeY = GRID_HEIGHT / 2;
        tailX.clear();
        tailY.clear();
        tailX.resize(500, -1);
        tailY.resize(500, -1);
        nTail = 0;
        score = 0;
        dir = STOP;
        gameOver = false;
        paused = false;
        invincible = false;
        doublePoints = false;
        gameSpeed = 10;
        lastMoveTime = 0;
        powerUp.active = false;
        
        spawnFruit();
    }
    
    bool run() {
        if (!init()) {
            return false;
        }
        
        reset();
        
        SDL_Event event;
        bool quit = false;
        
        while (!quit) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    quit = true;
                }
                handleInput(event);
            }
            
            if (gameOver && event.type != SDL_KEYDOWN) {
                // Wait for restart or quit
            } else {
                logic();
            }
            
            render();
            SDL_Delay(16); // ~60 FPS rendering
        }
        
        return true;
    }
};

int main(int argc, char* argv[]) {
    SnakeGameGUI game;
    
    if (!game.run()) {
        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR,
            "Error",
            "Failed to initialize game. Make sure SDL2 is installed.",
            nullptr
        );
        return 1;
    }
    
    return 0;
}
