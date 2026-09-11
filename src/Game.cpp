#include "Game.hpp"

#include "Constants.hpp"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <string>

Game::Game() {
    InitWindow(Constants::screenWidth, Constants::screenHeight, Constants::windowTitle);
    SetTargetFPS(Constants::targetFps);
    SetExitKey(KEY_ESCAPE);

    InitAudioDevice();
    audioReady_ = IsAudioDeviceReady();
    if (audioReady_) {
        flapSound_ = makeTone(620.0f, 0.09f);
        scoreSound_ = makeTone(880.0f, 0.12f);
        hitSound_ = makeTone(180.0f, 0.22f);
    }

    loadHighScore();
}

Game::~Game() {
    if (audioReady_) {
        UnloadSound(flapSound_);
        UnloadSound(scoreSound_);
        UnloadSound(hitSound_);
        CloseAudioDevice();
    }
    CloseWindow();
}

void Game::run() {
    while (!WindowShouldClose()) {
        const float dt = std::min(GetFrameTime(), 0.033f);
        update(dt);
        draw();
    }
}

void Game::update(float dt) {
    handleInput();

    const bool moving = state_ != GameState::GameOver;
    if (moving) {
        groundOffset_ += Constants::pipeSpeed * dt;
        cloudOffset_ += Constants::pipeSpeed * 0.25f * dt;
    }

    bird_.update(dt, state_ == GameState::Playing);

    if (state_ == GameState::Playing) {
        if (pipes_.empty() || pipes_.back().x() < Constants::screenWidth - Constants::pipeSpacing) {
            spawnPipe();
        }

        for (auto& pipe : pipes_) {
            pipe.update(dt, Constants::pipeSpeed);
            if (pipe.passed(Constants::birdX)) {
                pipe.markScored();
                ++score_;
                if (audioReady_) {
                    PlaySound(scoreSound_);
                }
            }
        }

        pipes_.erase(std::remove_if(pipes_.begin(), pipes_.end(),
                                    [](const PipePair& pipe) { return pipe.isOffScreen(); }),
                     pipes_.end());

        checkCollisions();
    }

    if (state_ == GameState::GameOver && restartDelay_ > 0.0f) {
        restartDelay_ -= dt;
    }
}

void Game::handleInput() {
    const bool pressed = IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    if (!pressed) {
        return;
    }

    if (state_ == GameState::Menu) {
        startRun();
        return;
    }

    if (state_ == GameState::Playing) {
        bird_.flap();
        if (audioReady_) {
            PlaySound(flapSound_);
        }
        return;
    }

    if (state_ == GameState::GameOver && restartDelay_ <= 0.0f) {
        startRun();
    }
}

void Game::startRun() {
    bird_.reset();
    pipes_.clear();
    score_ = 0;
    state_ = GameState::Playing;
    bird_.flap();
    if (audioReady_) {
        PlaySound(flapSound_);
    }
}

void Game::spawnPipe() {
    const float minY = Constants::minGapY;
    const float maxY = Constants::maxGapY;
    const float gapY = minY + static_cast<float>(GetRandomValue(0, 1000)) / 1000.0f * (maxY - minY);
    pipes_.emplace_back(static_cast<float>(Constants::screenWidth + 20), gapY);
}

void Game::checkCollisions() {
    const Vector2 pos = bird_.position();
    const float radius = bird_.radius() * 0.82f;

    if (pos.y + radius >= Constants::skyHeight || pos.y - radius <= 0.0f) {
        state_ = GameState::GameOver;
        restartDelay_ = 0.45f;
        if (score_ > highScore_) {
            highScore_ = score_;
            saveHighScore();
        }
        if (audioReady_) {
            PlaySound(hitSound_);
        }
        return;
    }

    for (const auto& pipe : pipes_) {
        if (pipe.collides(pos, radius)) {
            state_ = GameState::GameOver;
            restartDelay_ = 0.45f;
            if (score_ > highScore_) {
                highScore_ = score_;
                saveHighScore();
            }
            if (audioReady_) {
                PlaySound(hitSound_);
            }
            return;
        }
    }
}

void Game::loadHighScore() {
    std::ifstream file(Constants::highScoreFile);
    if (file) {
        file >> highScore_;
    }
}

void Game::saveHighScore() const {
    std::ofstream file(Constants::highScoreFile);
    if (file) {
        file << highScore_;
    }
}

Sound Game::makeTone(float frequency, float seconds) const {
    const int sampleRate = 44100;
    const int frames = static_cast<int>(seconds * sampleRate);

    Wave wave{};
    wave.frameCount = frames;
    wave.sampleRate = sampleRate;
    wave.sampleSize = 16;
    wave.channels = 1;

    auto* samples = static_cast<short*>(MemAlloc(frames * static_cast<int>(sizeof(short))));
    for (int i = 0; i < frames; ++i) {
        const float t = static_cast<float>(i) / static_cast<float>(sampleRate);
        const float envelope = 1.0f - static_cast<float>(i) / static_cast<float>(frames);
        samples[i] = static_cast<short>(std::sinf(2.0f * PI * frequency * t) * 12000.0f * envelope);
    }
    wave.data = samples;

    const Sound sound = LoadSoundFromWave(wave);
    UnloadWave(wave);
    return sound;
}

void Game::draw() const {
    BeginDrawing();
    drawBackground();

    for (const auto& pipe : pipes_) {
        pipe.draw();
    }

    drawGround();
    bird_.draw();
    drawHud();
    EndDrawing();
}

void Game::drawBackground() const {
    DrawRectangleGradientV(0, 0, Constants::screenWidth, Constants::screenHeight, Color{112, 197, 206, 255},
                           Color{176, 226, 232, 255});

    const Color cloud{255, 255, 255, 210};
    const float wrap = static_cast<float>(Constants::screenWidth) + 160.0f;
    auto cloudX = [this, wrap](float base) {
        const float x = std::fmod(base - cloudOffset_, wrap);
        return x < 0.0f ? x + wrap : x;
    };

    DrawEllipse(static_cast<int>(cloudX(80.0f)), 90, 46, 22, cloud);
    DrawEllipse(static_cast<int>(cloudX(110.0f)), 90, 32, 18, cloud);
    DrawEllipse(static_cast<int>(cloudX(280.0f)), 150, 52, 24, cloud);
    DrawEllipse(static_cast<int>(cloudX(320.0f)), 150, 36, 18, cloud);
    DrawEllipse(static_cast<int>(cloudX(430.0f)), 70, 40, 18, cloud);
}

void Game::drawGround() const {
    const int y = static_cast<int>(Constants::skyHeight);
    DrawRectangle(0, y, Constants::screenWidth, static_cast<int>(Constants::groundHeight), Color{222, 216, 149, 255});
    DrawRectangle(0, y, Constants::screenWidth, 18, Color{116, 191, 46, 255});
    DrawRectangle(0, y + 18, Constants::screenWidth, 6, Color{84, 148, 32, 255});

    const int tile = 24;
    const int offset = static_cast<int>(groundOffset_) % tile;
    for (int x = -tile; x < Constants::screenWidth + tile; x += tile) {
        DrawRectangle(x - offset, y + 4, 12, 8, Color{140, 210, 64, 180});
    }

    DrawRectangle(0, Constants::screenHeight - 18, Constants::screenWidth, 18, Color{196, 176, 110, 255});
}

void Game::drawCenteredText(const std::string& text, int y, int size, Color color) const {
    const int width = MeasureText(text.c_str(), size);
    DrawText(text.c_str(), (Constants::screenWidth - width) / 2, y, size, color);
}

void Game::drawHud() const {
    if (state_ == GameState::Playing || state_ == GameState::GameOver) {
        const std::string scoreText = std::to_string(score_);
        const int size = 48;
        const int width = MeasureText(scoreText.c_str(), size);
        const int x = (Constants::screenWidth - width) / 2;
        DrawText(scoreText.c_str(), x + 3, 43, size, Color{0, 0, 0, 90});
        DrawText(scoreText.c_str(), x, 40, size, WHITE);
    }

    if (state_ == GameState::Menu) {
        DrawRectangle(0, 0, Constants::screenWidth, Constants::screenHeight, Color{0, 0, 0, 40});
        drawCenteredText("FLAPPY BIRD", 150, 42, WHITE);
        drawCenteredText("FLAPPY BIRD", 148, 42, Color{255, 214, 64, 255});
        drawCenteredText("Espaco, seta ou clique para voar", 430, 20, WHITE);

        if (highScore_ > 0) {
            drawCenteredText("Recorde: " + std::to_string(highScore_), 500, 22, Color{255, 236, 150, 255});
        }
    }

    if (state_ == GameState::GameOver) {
        const int panelX = 70;
        const int panelY = 180;
        const int panelW = Constants::screenWidth - 140;
        const int panelH = 260;

        DrawRectangle(0, 0, Constants::screenWidth, Constants::screenHeight, Color{0, 0, 0, 80});
        DrawRectangleRounded(Rectangle{static_cast<float>(panelX), static_cast<float>(panelY),
                                       static_cast<float>(panelW), static_cast<float>(panelH)},
                             0.12f, 8, Color{250, 245, 220, 240});
        DrawRectangleRoundedLines(Rectangle{static_cast<float>(panelX), static_cast<float>(panelY),
                                            static_cast<float>(panelW), static_cast<float>(panelH)},
                                  0.12f, 8, Color{90, 70, 30, 255});

        drawCenteredText("FIM DE JOGO", 205, 28, Color{180, 70, 40, 255});
        drawCenteredText("Pontos: " + std::to_string(score_), 260, 26, Color{50, 50, 50, 255});
        drawCenteredText("Recorde: " + std::to_string(highScore_), 300, 24, Color{90, 90, 90, 255});
        drawCenteredText("Espaco para reiniciar", 370, 18, Color{80, 80, 80, 255});
    }
}
