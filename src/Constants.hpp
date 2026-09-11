#pragma once

namespace Constants {
    inline constexpr int screenWidth = 480;
    inline constexpr int screenHeight = 720;
    inline constexpr int targetFps = 60;

    inline constexpr float gravity = 1450.0f;
    inline constexpr float jumpVelocity = -420.0f;
    inline constexpr float maxFallSpeed = 720.0f;

    inline constexpr float birdX = 128.0f;
    inline constexpr float birdRadius = 18.0f;
    inline constexpr float birdStartY = 320.0f;

    inline constexpr float groundHeight = 96.0f;
    inline constexpr float skyHeight = screenHeight - groundHeight;

    inline constexpr float pipeWidth = 74.0f;
    inline constexpr float pipeGap = 168.0f;
    inline constexpr float pipeSpeed = 185.0f;
    inline constexpr float pipeSpacing = 250.0f;
    inline constexpr float pipeCapHeight = 28.0f;
    inline constexpr float pipeCapExtra = 8.0f;
    inline constexpr float minGapY = 110.0f;
    inline constexpr float maxGapY = skyHeight - 110.0f;

    inline constexpr const char* windowTitle = "Flappy Bird";
    inline constexpr const char* highScoreFile = "highscore.txt";
}
