#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include "colors.h"

struct ScoreEntry {
    std::string playerName;
    int score;
    std::string gameMode;
    std::string difficulty;
    std::string date;
    
    ScoreEntry() = default;
    
    ScoreEntry(const std::string& name, int score, const std::string& mode, const std::string& diff, const std::string& date)
        : playerName(name), score(score), gameMode(mode), difficulty(diff), date(date) {}
};

class Leaderboard {
private:
    std::vector<ScoreEntry> scores;
    std::string filename = "leaderboard.txt";
    
public:
    Leaderboard() {
        loadScores();
    }
    
    void loadScores() {
        scores.clear();
        std::ifstream file(filename);
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                if (!line.empty()) {
                    // Format: name,score,mode,difficulty,date
                    size_t pos = 0;
                    std::vector<std::string> tokens;
                    
                    while (pos < line.length()) {
                        size_t nextPos = line.find(',', pos);
                        if (nextPos == std::string::npos) {
                            tokens.push_back(line.substr(pos));
                            break;
                        }
                        tokens.push_back(line.substr(pos, nextPos - pos));
                        pos = nextPos + 1;
                    }
                    
                    if (tokens.size() >= 5) {
                        scores.emplace_back(tokens[0], std::stoi(tokens[1]), tokens[2], tokens[3], tokens[4]);
                    }
                }
            }
            file.close();
        }
    }
    
    void saveScores() {
        std::ofstream file(filename);
        if (file.is_open()) {
            for (const auto& score : scores) {
                file << score.playerName << "," << score.score << "," 
                     << score.gameMode << "," << score.difficulty << "," << score.date << std::endl;
            }
            file.close();
        }
    }
    
    void addScore(const std::string& playerName, int score, const std::string& gameMode, const std::string& difficulty) {
        // Get current date (simplified)
        std::string date = "2024-01-01"; // In a real implementation, you'd get the actual date
        
        scores.emplace_back(playerName, score, gameMode, difficulty, date);
        
        // Sort by score (descending)
        std::sort(scores.begin(), scores.end(), 
            [](const ScoreEntry& a, const ScoreEntry& b) {
                return a.score > b.score;
            });
        
        // Keep only top 10 scores
        if (scores.size() > 10) {
            scores.resize(10);
        }
        
        saveScores();
    }
    
    void displayLeaderboard() {
        system("clear");
        
        std::cout << GameColors::MENU_TITLE << "LEADERBOARD" << Colors::RESET << std::endl;
        std::cout << std::string(20, '=') << std::endl << std::endl;
        
        if (scores.empty()) {
            std::cout << GameColors::MENU_OPTION << "No scores recorded yet!" << Colors::RESET << std::endl;
            std::cout << GameColors::MENU_OPTION << "Play a game to set the first record!" << Colors::RESET << std::endl;
        } else {
            std::cout << GameColors::MENU_OPTION << std::left 
                      << std::setw(3) << "Rank" 
                      << std::setw(15) << "Player" 
                      << std::setw(8) << "Score" 
                      << std::setw(12) << "Mode" 
                      << std::setw(10) << "Difficulty" << Colors::RESET << std::endl;
            std::cout << std::string(50, '-') << std::endl;
            
            for (size_t i = 0; i < scores.size(); i++) {
                const auto& score = scores[i];
                std::cout << GameColors::SCORE << std::left 
                          << std::setw(3) << (i + 1)
                          << std::setw(15) << score.playerName
                          << std::setw(8) << score.score
                          << std::setw(12) << score.gameMode
                          << std::setw(10) << score.difficulty << Colors::RESET << std::endl;
            }
        }
        
        std::cout << std::endl << GameColors::MENU_OPTION << "Press ENTER to go back..." << Colors::RESET << std::endl;
        while (getchar() != '\n');
    }
    
    int getHighScore() const {
        return scores.empty() ? 0 : scores[0].score;
    }
    
    bool isNewRecord(int score) const {
        return scores.empty() || score > scores[0].score;
    }
};

#endif // LEADERBOARD_H