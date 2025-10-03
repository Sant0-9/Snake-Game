#ifndef SOUND_H
#define SOUND_H

#include <iostream>
#include <thread>
#include <chrono>
#include "config.h"

class SoundManager {
private:
    bool soundEnabled;
    GameConfig& config;
    
public:
    SoundManager(GameConfig& config) : config(config) {
        soundEnabled = config.getBool("sound_enabled");
    }
    
    void playBeep(int frequency = 1000, int duration = 100) {
        if (!soundEnabled) return;
        
        std::thread([frequency, duration]() {
            // Use system beep command
            std::string command = "echo -e \"\\a\"";
            system(command.c_str());
        }).detach();
    }
    
    void playEatSound() {
        if (!soundEnabled) return;
        
        std::thread([]() {
            // Play a sequence of beeps for eating sound
            for (int i = 0; i < 3; i++) {
                system("echo -e \"\\a\"");
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
        }).detach();
    }
    
    void playPowerUpSound() {
        if (!soundEnabled) return;
        
        std::thread([]() {
            // Play ascending beeps for power-up
            for (int i = 0; i < 5; i++) {
                system("echo -e \"\\a\"");
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }).detach();
    }
    
    void playGameOverSound() {
        if (!soundEnabled) return;
        
        std::thread([]() {
            // Play descending beeps for game over
            for (int i = 0; i < 3; i++) {
                system("echo -e \"\\a\"");
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
            }
        }).detach();
    }
    
    void playMenuSound() {
        if (!soundEnabled) return;
        
        std::thread([]() {
            system("echo -e \"\\a\"");
        }).detach();
    }
    
    void toggleSound() {
        soundEnabled = !soundEnabled;
        config.setBool("sound_enabled", soundEnabled);
        config.saveToFile();
    }
    
    bool isSoundEnabled() const {
        return soundEnabled;
    }
};

#endif // SOUND_H