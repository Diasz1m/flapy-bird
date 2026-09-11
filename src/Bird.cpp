#include "Bird.hpp"

#include "Constants.hpp"

#include <algorithm>
#include <cmath>

Bird::Bird() {
    reset();
}

void Bird::reset() {
    position_ = {Constants::birdX, Constants::birdStartY};
    velocity_ = 0.0f;
    radius_ = Constants::birdRadius;
    rotation_ = 0.0f;
    wingTime_ = 0.0f;
}

void Bird::flap() {
    velocity_ = Constants::jumpVelocity;
}

void Bird::update(float dt, bool physicsEnabled) {
    wingTime_ += dt;

    if (physicsEnabled) {
        velocity_ += Constants::gravity * dt;
        velocity_ = std::min(velocity_, Constants::maxFallSpeed);
        position_.y += velocity_ * dt;
        rotation_ = std::clamp(velocity_ * 0.12f, -28.0f, 90.0f);
    } else {
        position_.y = Constants::birdStartY + std::sinf(wingTime_ * 3.2f) * 10.0f;
        rotation_ = std::sinf(wingTime_ * 3.2f) * 8.0f;
        velocity_ = 0.0f;
    }
}

void Bird::draw() const {
    const Vector2 center = position_;
    const float deg = rotation_;

    const Vector2 shadow{center.x + 3.0f, center.y + 8.0f};
    DrawEllipse(static_cast<int>(shadow.x), static_cast<int>(shadow.y), radius_ * 1.05f, radius_ * 0.55f,
                Color{0, 0, 0, 40});

    const Color body{255, 214, 64, 255};
    const Color bodyDark{232, 176, 32, 255};
    const Color wing{245, 245, 245, 255};
    const Color beak{255, 138, 42, 255};
    const Color beakDark{214, 96, 24, 255};

    DrawCircleV(center, radius_ + 1.5f, bodyDark);
    DrawCircleV(center, radius_, body);
    DrawCircleV({center.x - 3.0f, center.y + 5.0f}, radius_ * 0.55f, Color{255, 236, 150, 255});

    const float wingAngle = std::sinf(wingTime_ * 18.0f) * 28.0f + deg * 0.3f;
    const float wingRad = wingAngle * DEG2RAD;
    const Vector2 wingCenter{
        center.x - 4.0f + std::cosf(wingRad) * 2.0f,
        center.y + 2.0f + std::sinf(wingRad) * 6.0f,
    };
    DrawEllipse(static_cast<int>(wingCenter.x), static_cast<int>(wingCenter.y), 12.0f, 8.0f, wing);
    DrawEllipseLines(static_cast<int>(wingCenter.x), static_cast<int>(wingCenter.y), 12.0f, 8.0f,
                     Color{200, 200, 200, 255});

    const float rot = deg * DEG2RAD;
    const Vector2 beakTip{
        center.x + std::cosf(rot) * (radius_ + 10.0f),
        center.y + std::sinf(rot) * (radius_ + 10.0f) + 2.0f,
    };
    const Vector2 beakTop{
        center.x + std::cosf(rot - 0.55f) * (radius_ - 2.0f),
        center.y + std::sinf(rot - 0.55f) * (radius_ - 2.0f),
    };
    const Vector2 beakBot{
        center.x + std::cosf(rot + 0.55f) * (radius_ - 2.0f),
        center.y + std::sinf(rot + 0.55f) * (radius_ - 2.0f),
    };
    DrawTriangle(beakTip, beakTop, beakBot, beak);
    DrawLineEx(beakTop, beakBot, 1.5f, beakDark);

    const Vector2 eye{
        center.x + 6.0f,
        center.y - 5.0f,
    };
    DrawCircleV(eye, 6.5f, WHITE);
    DrawCircleV({eye.x + 2.0f, eye.y}, 3.2f, BLACK);
    DrawCircleV({eye.x + 3.2f, eye.y - 1.4f}, 1.2f, WHITE);
}
