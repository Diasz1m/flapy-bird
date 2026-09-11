#pragma once

#include "raylib.h"

class PipePair {
public:
    PipePair(float startX, float gapCenterY);

    void update(float dt, float speed);
    void draw() const;
    bool collides(Vector2 center, float radius) const;
    bool isOffScreen() const;
    bool passed(float birdX) const;

    float x() const { return x_; }
    bool scored() const { return scored_; }
    void markScored() { scored_ = true; }

private:
    Rectangle topRect() const;
    Rectangle bottomRect() const;
    static void drawPipe(const Rectangle& body, bool capAtBottom);

    float x_;
    float gapY_;
    bool scored_ = false;
};
