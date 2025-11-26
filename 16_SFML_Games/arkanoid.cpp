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

// Handle ball–block collisions (affects dx or dy)
void handleBallBlockCollision(float ballX, float ballY, float& ballSpeed,
    Sprite blockSprites[], int blockCount)
{
    FloatRect ballRect(ballX + 3, ballY + 3, 6, 6);

    for (int i = 0; i < blockCount; i++)
    {
        if (ballRect.intersects(blockSprites[i].getGlobalBounds()))
        {
            blockSprites[i].setPosition(-100, 0);
            ballSpeed = -ballSpeed;
            return; // only one bounce per frame
        }
    }
}

// handle bouncing from window borders
void handleWallCollision(float ballX, float ballY, float& ballDx, float& ballDy)
{
    if (ballX < 0 || ballX > WINDOW_WIDTH)  ballDx = -ballDx;
    if (ballY < 0 || ballY > WINDOW_HEIGHT) ballDy = -ballDy;
}

// handle user input for paddle movement
void updatePaddleMovement(Sprite& paddleSprite)
{
    if (Keyboard::isKeyPressed(Keyboard::Right))
        paddleSprite.move(PADDLE_SPEED, 0);

    if (Keyboard::isKeyPressed(Keyboard::Left))
        paddleSprite.move(-PADDLE_SPEED, 0);
}

// handle paddle–ball collision
void handlePaddleCollision(float ballX, float ballY, float& ballDy, const Sprite& paddleSprite)
{
    if (FloatRect(ballX, ballY, BALL_SIZE, BALL_SIZE).intersects(paddleSprite.getGlobalBounds()))
        ballDy = -(rand() % 5 + 2);
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

    Sprite backgroundSprite(backgroundTexture), ballSprite(ballTexture), paddleSprite(paddleTexture);
    paddleSprite.setPosition(PADDLE_START_X, PADDLE_START_Y);

    Sprite blockSprites[1000];

    int blockCount = 0;
    for (int col = 1; col <= BLOCK_COLS; col++)
        for (int row = 1; row <= BLOCK_ROWS; row++)
        {
            blockSprites[blockCount].setTexture(blockTexture);
            blockSprites[blockCount].setPosition(col * BLOCK_SPACING_X, row * BLOCK_SPACING_Y);
            blockCount++;
        }

    float ballDx = BALL_SPEED_X, ballDy = BALL_SPEED_Y;
    float ballX = BALL_START_X, ballY = BALL_START_Y;

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        // ball movement
        ballX += ballDx;
        handleBallBlockCollision(ballX, ballY, ballDx, blockSprites, blockCount);

        ballY += ballDy;
        handleBallBlockCollision(ballX, ballY, ballDy, blockSprites, blockCount);

        // wall collision
        handleWallCollision(ballX, ballY, ballDx, ballDy);

        // paddle movement
        updatePaddleMovement(paddleSprite);

        //paddle collision
        handlePaddleCollision(ballX, ballY, ballDy, paddleSprite);

        ballSprite.setPosition(ballX, ballY);

        window.clear();
        window.draw(backgroundSprite);
        window.draw(ballSprite);
        window.draw(paddleSprite);

        for (int i = 0; i < blockCount; i++)
            window.draw(blockSprites[i]);

        window.display();
    }

    return 0;
}
