#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <fstream>
#include <iostream>
#include <map>

class GameConfig {
private:
    std::map<std::string, std::string> settings;
    std::string configFile = "snake_config.txt";
    
public:
    GameConfig() {
        loadDefaultSettings();
        loadFromFile();
    }
    
    void loadDefaultSettings() {
        settings["player_name"] = "Player";
        settings["difficulty"] = "normal";
        settings["game_mode"] = "classic";
        settings["sound_enabled"] = "true";
        settings["colors_enabled"] = "true";
        settings["board_width"] = "30";
        settings["board_height"] = "30";
        settings["speed_multiplier"] = "1.0";
        settings["power_ups_enabled"] = "true";
        settings["high_score"] = "0";
    }
    
    void loadFromFile() {
        std::ifstream file(configFile);
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                size_t pos = line.find('=');
                if (pos != std::string::npos) {
                    std::string key = line.substr(0, pos);
                    std::string value = line.substr(pos + 1);
                    settings[key] = value;
                }
            }
            file.close();
        }
    }
    
    void saveToFile() {
        std::ofstream file(configFile);
        if (file.is_open()) {
            for (const auto& setting : settings) {
                file << setting.first << "=" << setting.second << std::endl;
            }
            file.close();
        }
    }
    
    std::string getString(const std::string& key) const {
        auto it = settings.find(key);
        return (it != settings.end()) ? it->second : "";
    }
    
    int getInt(const std::string& key) const {
        std::string value = getString(key);
        return value.empty() ? 0 : std::stoi(value);
    }
    
    bool getBool(const std::string& key) const {
        std::string value = getString(key);
        return value == "true";
    }
    
    double getDouble(const std::string& key) const {
        std::string value = getString(key);
        return value.empty() ? 0.0 : std::stod(value);
    }
    
    void setString(const std::string& key, const std::string& value) {
        settings[key] = value;
    }
    
    void setInt(const std::string& key, int value) {
        settings[key] = std::to_string(value);
    }
    
    void setBool(const std::string& key, bool value) {
        settings[key] = value ? "true" : "false";
    }
    
    void setDouble(const std::string& key, double value) {
        settings[key] = std::to_string(value);
    }
};

// Game modes enum
enum class GameMode {
    CLASSIC,
    SPEED,
    SURVIVAL,
    INFINITE
};

// Difficulty levels
enum class Difficulty {
    EASY,
    NORMAL,
    HARD,
    EXPERT
};

#endif // CONFIG_H