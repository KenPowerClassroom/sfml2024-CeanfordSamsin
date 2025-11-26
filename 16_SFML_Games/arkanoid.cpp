#include <SFML/Graphics.hpp>
#include <time.h>

using namespace sf;

const int WINDOW_WIDTH = 520;
const int WINDOW_HEIGHT = 450;

const int BLOCK_COLS = 10;
const int BLOCK_ROWS = 10;
const float BLOCK_SPACING_X = 43.f;
const float BLOCK_SPACING_Y = 20.f;

const float BALL_START_X = 300.f;
const float BALL_START_Y = 300.f;
const float BALL_SPEED_X = 6.f;
const float BALL_SPEED_Y = 5.f;
const float BALL_SIZE = 12.f;

const float PADDLE_START_X = 300.f;
const float PADDLE_START_Y = 440.f;
const float PADDLE_SPEED = 6.f;

// Ball object
struct Ball {
    float x, y;
    float dx, dy;
    Sprite sprite;

    Ball(float startX, float startY, float speedX, float speedY, const Texture& texture)
        : x(startX), y(startY), dx(speedX), dy(speedY), sprite(texture) {
    }

    FloatRect getHitbox() const {
        // A smaller inner hitbox used for block collision detection
        return FloatRect(x + 3.f, y + 3.f, 6.f, 6.f);
    }

    FloatRect getBounds() const {
        // Full size of the ball used for paddle collision
        return FloatRect(x, y, BALL_SIZE, BALL_SIZE);
    }

    void updateSprite() {
        sprite.setPosition(x, y);
    }
};

// Paddle object
struct Paddle {
    Sprite sprite;

    Paddle(float startX, float startY, const Texture& texture) : sprite(texture) {
        sprite.setPosition(startX, startY);
    }

    void moveLeft() {
        sprite.move(-PADDLE_SPEED, 0);
    }

    void moveRight() {
        sprite.move(PADDLE_SPEED, 0);
    }

    FloatRect getBounds() const {
        return sprite.getGlobalBounds();
    }
};

// Handle ball–block collisions (affects dx or dy)
void handleBallBlockCollision(Ball& ball, Sprite blockSprites[], int blockCount, bool horizontal) {
    FloatRect ballHitbox = ball.getHitbox();

    // Loop through all blocks to see if the ball hit one
    for (int i = 0; i < blockCount; i++) {
        const FloatRect blockBounds = blockSprites[i].getGlobalBounds();

        // If the ball intersects this block, the block "breaks"
        if (ballHitbox.intersects(blockBounds)) {
            blockSprites[i].setPosition(-100, 0); // move block offscreen
            if (horizontal)
                ball.dx = -ball.dx;  // bounce horizontally
            else
                ball.dy = -ball.dy;  // bounce vertically
            return; // only one bounce per frame
        }
    }
}

// handle bouncing from window borders
void handleWallCollision(Ball& ball) {
    // Bounce horizontally
    if (ball.x < 0 || ball.x > WINDOW_WIDTH)  ball.dx = -ball.dx;
    // Bounce vertically
    if (ball.y < 0 || ball.y > WINDOW_HEIGHT) ball.dy = -ball.dy;
}

// handle user input for paddle movement
void updatePaddleMovement(Paddle& paddle) {
    // Move paddle right
    if (Keyboard::isKeyPressed(Keyboard::Right))
        paddle.moveRight();

    // Move paddle left
    if (Keyboard::isKeyPressed(Keyboard::Left))
        paddle.moveLeft();
}

// handle paddle–ball collision
void handlePaddleCollision(Ball& ball, const Paddle& paddle) {
    // If the ball hits the paddle, bounce upward
    if (ball.getBounds().intersects(paddle.getBounds()))
        ball.dy = -(rand() % 5 + 2); // random upward angle
}

int arkanoid() {
    srand(time(0));

    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Arkanoid!");
    window.setFramerateLimit(60);

    Texture blockTexture, backgroundTexture, ballTexture, paddleTexture;
    blockTexture.loadFromFile("images/arkanoid/block01.png");
    backgroundTexture.loadFromFile("images/arkanoid/background.jpg");
    ballTexture.loadFromFile("images/arkanoid/ball.png");
    paddleTexture.loadFromFile("images/arkanoid/paddle.png");

    Sprite backgroundSprite(backgroundTexture);

    // Create objects
    Ball ball(BALL_START_X, BALL_START_Y, BALL_SPEED_X, BALL_SPEED_Y, ballTexture);
    Paddle paddle(PADDLE_START_X, PADDLE_START_Y, paddleTexture);

    Sprite blockSprites[1000];
    int blockCount = 0;

    // Create a 10x10 grid of blocks
    for (int col = 1; col <= BLOCK_COLS; col++) {
        for (int row = 1; row <= BLOCK_ROWS; row++) {
            blockSprites[blockCount].setTexture(blockTexture);
            blockSprites[blockCount].setPosition(
                col * BLOCK_SPACING_X,
                row * BLOCK_SPACING_Y
            );
            blockCount++;  // count total blocks created
        }
    }

    while (window.isOpen()) {
        Event event;

        // Process window events
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)  // user closed the window
                window.close();
        }

        // Move ball horizontally and check collisions
        ball.x += ball.dx;
        handleBallBlockCollision(ball, blockSprites, blockCount, true);

        // Move ball vertically and check collisions
        ball.y += ball.dy;
        handleBallBlockCollision(ball, blockSprites, blockCount, false);

        // Bounce off walls (edges of the screen)
        handleWallCollision(ball);

        // Move paddle left/right depending on key input
        updatePaddleMovement(paddle);

        // Bounce ball off the paddle if they intersect
        handlePaddleCollision(ball, paddle);

        // Update ball sprite position to match physics
        ball.updateSprite();

        window.clear();
        window.draw(backgroundSprite);
        window.draw(ball.sprite);
        window.draw(paddle.sprite);

        // Draw every active block
        for (int i = 0; i < blockCount; i++)
            window.draw(blockSprites[i]);

        window.display();
    }

    return 0;
}
