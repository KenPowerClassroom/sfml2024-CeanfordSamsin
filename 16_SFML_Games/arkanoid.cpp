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

int arkanoid()
{
    srand(time(0));

    RenderWindow app(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Arkanoid!");
    app.setFramerateLimit(60);

    Texture t1, t2, t3, t4;
    t1.loadFromFile("images/arkanoid/block01.png");
    t2.loadFromFile("images/arkanoid/background.jpg");
    t3.loadFromFile("images/arkanoid/ball.png");
    t4.loadFromFile("images/arkanoid/paddle.png");

    Sprite sBackground(t2), sBall(t3), sPaddle(t4);
    sPaddle.setPosition(PADDLE_START_X, PADDLE_START_Y);

    Sprite block[1000];

    int n = 0;
    for (int i = 1; i <= BLOCK_COLS; i++)
        for (int j = 1; j <= BLOCK_ROWS; j++)
        {
            block[n].setTexture(t1);
            block[n].setPosition(i * BLOCK_SPACING_X, j * BLOCK_SPACING_Y);
            n++;
        }

    float dx = BALL_SPEED_X, dy = BALL_SPEED_Y;
    float x = BALL_START_X, y = BALL_START_Y;

    while (app.isOpen())
    {
        Event e;
        while (app.pollEvent(e))
        {
            if (e.type == Event::Closed)
                app.close();
        }

        x += dx;
        for (int i = 0; i < n; i++)
            if (FloatRect(x + 3, y + 3, 6, 6).intersects(block[i].getGlobalBounds()))
            {
                block[i].setPosition(-100, 0); dx = -dx;
            }

        y += dy;
        for (int i = 0; i < n; i++)
            if (FloatRect(x + 3, y + 3, 6, 6).intersects(block[i].getGlobalBounds()))
            {
                block[i].setPosition(-100, 0); dy = -dy;
            }

        if (x<0 || x>WINDOW_WIDTH)  dx = -dx;
        if (y<0 || y>WINDOW_HEIGHT) dy = -dy;

        if (Keyboard::isKeyPressed(Keyboard::Right)) sPaddle.move(PADDLE_SPEED, 0);
        if (Keyboard::isKeyPressed(Keyboard::Left))  sPaddle.move(-PADDLE_SPEED, 0);

        if (FloatRect(x, y, BALL_SIZE, BALL_SIZE).intersects(sPaddle.getGlobalBounds()))
            dy = -(rand() % 5 + 2);

        sBall.setPosition(x, y);

        app.clear();
        app.draw(sBackground);
        app.draw(sBall);
        app.draw(sPaddle);

        for (int i = 0; i < n; i++)
            app.draw(block[i]);

        app.display();
    }

    return 0;
}
