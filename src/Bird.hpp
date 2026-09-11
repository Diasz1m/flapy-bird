#pragma once

#include "raylib.h"

class Bird {
public:
    Bird();

    void reset();
    void flap();
    void update(float dt, bool physicsEnabled);
    void draw() const;

    Vector2 position() const { return position_; }
    float radius() const { return radius_; }
    float rotation() const { return rotation_; }

private:
    Vector2 position_{};
    float velocity_ = 0.0f;
    float radius_ = 0.0f;
    float rotation_ = 0.0f;
    float wingTime_ = 0.0f;
};
