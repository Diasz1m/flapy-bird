#include "Pipe.hpp"

#include "Constants.hpp"

PipePair::PipePair(float startX, float gapCenterY)
    : x_(startX),
      gapY_(gapCenterY) {}

void PipePair::update(float dt, float speed) {
    x_ -= speed * dt;
}

Rectangle PipePair::topRect() const {
    const float topHeight = gapY_ - Constants::pipeGap * 0.5f;
    return {x_, 0.0f, Constants::pipeWidth, topHeight};
}

Rectangle PipePair::bottomRect() const {
    const float gapBottom = gapY_ + Constants::pipeGap * 0.5f;
    return {x_, gapBottom, Constants::pipeWidth, Constants::skyHeight - gapBottom};
}

void PipePair::drawPipe(const Rectangle& body, bool capAtBottom) {
    const Color fill{82, 196, 61, 255};
    const Color shade{54, 150, 40, 255};
    const Color highlight{164, 230, 96, 255};
    const Color outline{20, 90, 24, 255};

    DrawRectangleRec(body, fill);
    DrawRectangle(static_cast<int>(body.x + body.width - 16.0f), static_cast<int>(body.y), 16,
                  static_cast<int>(body.height), shade);
    DrawRectangle(static_cast<int>(body.x + 6.0f), static_cast<int>(body.y), 8, static_cast<int>(body.height),
                  highlight);
    DrawRectangleLinesEx(body, 2.0f, outline);

    Rectangle cap = body;
    cap.x -= Constants::pipeCapExtra;
    cap.width += Constants::pipeCapExtra * 2.0f;
    cap.height = Constants::pipeCapHeight;
    if (capAtBottom) {
        cap.y = body.y + body.height - Constants::pipeCapHeight;
    }

    DrawRectangleRec(cap, fill);
    DrawRectangle(static_cast<int>(cap.x + cap.width - 18.0f), static_cast<int>(cap.y), 18,
                  static_cast<int>(cap.height), shade);
    DrawRectangleLinesEx(cap, 2.5f, outline);
}

void PipePair::draw() const {
    drawPipe(topRect(), true);
    drawPipe(bottomRect(), false);
}

bool PipePair::collides(Vector2 center, float radius) const {
    return CheckCollisionCircleRec(center, radius, topRect()) ||
           CheckCollisionCircleRec(center, radius, bottomRect());
}

bool PipePair::isOffScreen() const {
    return x_ + Constants::pipeWidth + Constants::pipeCapExtra < 0.0f;
}

bool PipePair::passed(float birdX) const {
    return !scored_ && (x_ + Constants::pipeWidth) < birdX;
}
