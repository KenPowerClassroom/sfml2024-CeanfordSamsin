#pragma once
#include <SFML/Graphics.hpp>

struct BallLogicBall {
    float x, y;
    float dx, dy;

    BallLogicBall(float x_, float y_, float dx_, float dy_)
        : x(x_), y(y_), dx(dx_), dy(dy_) {
    }

    sf::FloatRect getHitbox() const {
        return sf::FloatRect(x + 3.f, y + 3.f, 6.f, 6.f);
    }

    sf::FloatRect getBounds() const {
        return sf::FloatRect(x, y, 12.f, 12.f);
    }
};

inline void BallLogic_HandleWallCollision(BallLogicBall& ball) {
    if (ball.x < 0 || ball.x > 520) ball.dx = -ball.dx;
    if (ball.y < 0 || ball.y > 450) ball.dy = -ball.dy;
}
