#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;

struct Ball {
    float x, y;
    float dx, dy;
    Sprite sprite;

    Ball(float x_, float y_, float dx_, float dy_, const Texture& texture)
        : x(x_), y(y_), dx(dx_), dy(dy_), sprite(texture) {
    }

    FloatRect getHitbox() const { return FloatRect(x + 3.f, y + 3.f, 6.f, 6.f); }
    FloatRect getBounds() const { return FloatRect(x, y, 12.f, 12.f); }
    void updateSprite() { sprite.setPosition(x, y); }
};

struct Paddle {
    Sprite sprite;

    Paddle(float x, float y, const Texture& texture) : sprite(texture) {
        sprite.setPosition(x, y);
    }

    FloatRect getBounds() const { return sprite.getGlobalBounds(); }
};

void handleBallBlockCollision(Ball& ball, Sprite blocks[], int blockCount, bool horizontal);
void handleWallCollision(Ball& ball);
void handlePaddleCollision(Ball& ball, const Paddle& paddle);
