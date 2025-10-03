#ifndef POWERUPS_H
#define POWERUPS_H

#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include "colors.h"

enum class PowerUpType {
    SPEED_BOOST,
    SLOW_DOWN,
    DOUBLE_POINTS,
    INVINCIBLE,
    SHRINK,
    GROWTH_BOOST,
    TELEPORT
};

struct PowerUp {
    int x, y;
    PowerUpType type;
    std::chrono::steady_clock::time_point spawnTime;
    bool active;
    
    PowerUp(int x, int y, PowerUpType type) 
        : x(x), y(y), type(type), active(true) {
        spawnTime = std::chrono::steady_clock::now();
    }
    
    bool isExpired() const {
        auto now = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - spawnTime);
        return duration.count() > 10; // Power-ups expire after 10 seconds
    }
    
    char getSymbol() const {
        switch (type) {
            case PowerUpType::SPEED_BOOST: return 'S';
            case PowerUpType::SLOW_DOWN: return 'L';
            case PowerUpType::DOUBLE_POINTS: return 'D';
            case PowerUpType::INVINCIBLE: return 'I';
            case PowerUpType::SHRINK: return 'R';
            case PowerUpType::GROWTH_BOOST: return 'G';
            case PowerUpType::TELEPORT: return 'T';
            default: return '?';
        }
    }
    
    const char* getColor() const {
        switch (type) {
            case PowerUpType::SPEED_BOOST: return Colors::BOLD_CYAN;
            case PowerUpType::SLOW_DOWN: return Colors::BOLD_BLUE;
            case PowerUpType::DOUBLE_POINTS: return Colors::BOLD_YELLOW;
            case PowerUpType::INVINCIBLE: return Colors::BOLD_WHITE;
            case PowerUpType::SHRINK: return Colors::BOLD_MAGENTA;
            case PowerUpType::GROWTH_BOOST: return Colors::BOLD_GREEN;
            case PowerUpType::TELEPORT: return Colors::BOLD_RED;
            default: return Colors::WHITE;
        }
    }
    
    std::string getDescription() const {
        switch (type) {
            case PowerUpType::SPEED_BOOST: return "Speed Boost - Move faster for 5 seconds";
            case PowerUpType::SLOW_DOWN: return "Slow Down - Move slower for 5 seconds";
            case PowerUpType::DOUBLE_POINTS: return "Double Points - 2x score for 10 seconds";
            case PowerUpType::INVINCIBLE: return "Invincible - No collision damage for 3 seconds";
            case PowerUpType::SHRINK: return "Shrink - Reduce snake length by 3";
            case PowerUpType::GROWTH_BOOST: return "Growth Boost - Grow by 5 segments";
            case PowerUpType::TELEPORT: return "Teleport - Random position on board";
            default: return "Unknown Power-up";
        }
    }
};

class PowerUpManager {
private:
    std::vector<PowerUp> powerUps;
    std::mt19937 rng;
    int boardWidth, boardHeight;
    double spawnChance = 0.15; // 15% chance per fruit eaten
    
public:
    PowerUpManager(int width, int height) 
        : rng(std::random_device{}()), boardWidth(width), boardHeight(height) {}
    
    void update() {
        // Remove expired power-ups
        powerUps.erase(
            std::remove_if(powerUps.begin(), powerUps.end(),
                [](const PowerUp& p) { return p.isExpired(); }),
            powerUps.end()
        );
    }
    
    void trySpawnPowerUp(int /*fruitX*/, int /*fruitY*/, const std::vector<int>& tailX, const std::vector<int>& tailY, int nTail) {
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        if (dist(rng) < spawnChance) {
            spawnRandomPowerUp(tailX, tailY, nTail);
        }
    }
    
    void spawnRandomPowerUp(const std::vector<int>& tailX, const std::vector<int>& tailY, int nTail) {
        std::uniform_int_distribution<int> xDist(0, boardWidth - 1);
        std::uniform_int_distribution<int> yDist(0, boardHeight - 1);
        std::uniform_int_distribution<int> typeDist(0, 6);
        
        int attempts = 0;
        while (attempts < 50) { // Prevent infinite loop
            int x = xDist(rng);
            int y = yDist(rng);
            
            // Check if position is free
            bool free = true;
            for (int i = 0; i < nTail; i++) {
                if (tailX[i] == x && tailY[i] == y) {
                    free = false;
                    break;
                }
            }
            
            // Check if no other power-up is at this position
            for (const auto& powerUp : powerUps) {
                if (powerUp.x == x && powerUp.y == y) {
                    free = false;
                    break;
                }
            }
            
            if (free) {
                PowerUpType type = static_cast<PowerUpType>(typeDist(rng));
                powerUps.emplace_back(x, y, type);
                break;
            }
            attempts++;
        }
    }
    
    PowerUp* checkCollision(int x, int y) {
        for (auto& powerUp : powerUps) {
            if (powerUp.x == x && powerUp.y == y && powerUp.active) {
                powerUp.active = false;
                return &powerUp;
            }
        }
        return nullptr;
    }
    
    const std::vector<PowerUp>& getPowerUps() const {
        return powerUps;
    }
    
    void clear() {
        powerUps.clear();
    }
};

#endif // POWERUPS_H