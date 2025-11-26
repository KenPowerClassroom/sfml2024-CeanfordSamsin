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

static FloatRect getBallHitbox(float x, float y)
{
    // A smaller inner hitbox used for block collision detection
    return FloatRect(x + 3.f, y + 3.f, 6.f, 6.f);
}

static FloatRect getBallBounds(float x, float y)
{
    // Full size of the ball used for paddle collision
    return FloatRect(x, y, BALL_SIZE, BALL_SIZE);
}

void handleBallBlockCollision(float ballX, float ballY, float& ballSpeed,
    Sprite blockSprites[], int blockCount)
{
    FloatRect ballHitbox = getBallHitbox(ballX, ballY);

    // Loop through all blocks to see if the ball hit one
    for (int i = 0; i < blockCount; i++)
    {
        const FloatRect blockBounds = blockSprites[i].getGlobalBounds();

        // If the ball intersects this block, the block "breaks"
        if (ballHitbox.intersects(blockBounds))
        {
            blockSprites[i].setPosition(-100, 0); // move block offscreen
            ballSpeed = -ballSpeed; // bounce in opposite direction
            return;
        }
    }
}

void handleWallCollision(float ballX, float ballY, float& ballDx, float& ballDy)
{
    bool hitLeftOrRight = (ballX < 0 || ballX > WINDOW_WIDTH);
    bool hitTopOrBottom = (ballY < 0 || ballY > WINDOW_HEIGHT);

    // Bounce horizontally
    if (hitLeftOrRight)
        ballDx = -ballDx;

    // Bounce vertically
    if (hitTopOrBottom)
        ballDy = -ballDy;
}

void updatePaddleMovement(Sprite& paddleSprite)
{
    // Move paddle right
    if (Keyboard::isKeyPressed(Keyboard::Right))
        paddleSprite.move(PADDLE_SPEED, 0);

    // Move paddle left
    if (Keyboard::isKeyPressed(Keyboard::Left))
        paddleSprite.move(-PADDLE_SPEED, 0);
}

void handlePaddleCollision(float ballX, float ballY, float& ballDy,
    const Sprite& paddleSprite)
{
    FloatRect ballBounds = getBallBounds(ballX, ballY);
    FloatRect paddleBounds = paddleSprite.getGlobalBounds();

    // If the ball hits the paddle, bounce upward
    if (ballBounds.intersects(paddleBounds))
        ballDy = -(rand() % 5 + 2); // random upward angle
}

int arkanoid()
{
    srand(time(0));

    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Arkanoid!");
    window.setFramerateLimit(60);

    Texture blockTexture, backgroundTexture, ballTexture, paddleTexture;
    blockTexture.loadFromFile("images/arkanoid/block01.png");
    backgroundTexture.loadFromFile("images/arkanoid/background.jpg");
    ballTexture.loadFromFile("images/arkanoid/ball.png");
    paddleTexture.loadFromFile("images/arkanoid/paddle.png");

    Sprite backgroundSprite(backgroundTexture);
    Sprite ballSprite(ballTexture);
    Sprite paddleSprite(paddleTexture);

    paddleSprite.setPosition(PADDLE_START_X, PADDLE_START_Y);

    Sprite blockSprites[1000];
    int blockCount = 0;

    // Create a 10x10 grid of blocks
    for (int col = 1; col <= BLOCK_COLS; col++)
    {
        for (int row = 1; row <= BLOCK_ROWS; row++)
        {
            blockSprites[blockCount].setTexture(blockTexture);
            blockSprites[blockCount].setPosition(
                col * BLOCK_SPACING_X,
                row * BLOCK_SPACING_Y
            );
            blockCount++;  // count total blocks created
        }
    }

    float ballDx = BALL_SPEED_X;
    float ballDy = BALL_SPEED_Y;
    float ballX = BALL_START_X;
    float ballY = BALL_START_Y;

    while (window.isOpen())
    {
        Event event;

        // Process window events
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)  // user closed the window
                window.close();
        }

        // Move ball horizontally
        ballX += ballDx;

        // Check block collisions in X direction
        handleBallBlockCollision(ballX, ballY, ballDx, blockSprites, blockCount);

        // Move ball vertically
        ballY += ballDy;

        // Check block collisions in Y direction
        handleBallBlockCollision(ballX, ballY, ballDy, blockSprites, blockCount);

        // Bounce off walls (edges of the screen)
        handleWallCollision(ballX, ballY, ballDx, ballDy);

        // Move paddle left/right depending on key input
        updatePaddleMovement(paddleSprite);

        // Bounce ball off the paddle if they intersect
        handlePaddleCollision(ballX, ballY, ballDy, paddleSprite);

        // Update ball sprite position to match physics
        ballSprite.setPosition(ballX, ballY);

        window.clear();
        window.draw(backgroundSprite);
        window.draw(ballSprite);
        window.draw(paddleSprite);

        // Draw every active block
        for (int i = 0; i < blockCount; i++)
            window.draw(blockSprites[i]);

        window.display();
    }

    return 0;
}
