#pragma once

#include "Bird.hpp"
#include "Pipe.hpp"

#include "raylib.h"

#include <string>
#include <vector>

enum class GameState {
    Menu,
    Playing,
    GameOver,
};

class Game {
public:
    Game();
    ~Game();

    void run();

private:
    void update(float dt);
    void draw() const;

    void startRun();
    void spawnPipe();
    void handleInput();
    void checkCollisions();
    void loadHighScore();
    void saveHighScore() const;

    void drawBackground() const;
    void drawGround() const;
    void drawHud() const;
    void drawCenteredText(const std::string& text, int y, int size, Color color) const;

    Sound makeTone(float frequency, float seconds) const;

    GameState state_ = GameState::Menu;
    Bird bird_;
    std::vector<PipePair> pipes_;
    int score_ = 0;
    int highScore_ = 0;
    float groundOffset_ = 0.0f;
    float cloudOffset_ = 0.0f;
    float restartDelay_ = 0.0f;
    bool audioReady_ = false;
    Sound flapSound_{};
    Sound scoreSound_{};
    Sound hitSound_{};
};
