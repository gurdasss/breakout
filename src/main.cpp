#include "raylib.h"
#include <vector> // std::vector

// CONSTANTS
// Game related
constexpr short g_screenWidth{800};
constexpr short g_screenHeight{450};
const char *g_gameTitle{"Breakout"};

constexpr short g_boxSize{25};
constexpr short g_fitScreenWidth{g_screenWidth / g_boxSize};
constexpr short g_fitScreenHeight{(g_screenHeight / 2) / g_boxSize};

struct Ball
{
    Vector2 position{};
    float radius{};
    Color color{};
};

struct Brick
{
    Rectangle brick{};
    Color color{};

    bool checkCollisionWithBall(const Ball &ball)
    {
        return CheckCollisionCircleRec(ball.position, ball.radius, brick);
    }
};

void updatePlayerPosition(Rectangle &player);
void checkBallCollision(Ball &ball, const Rectangle &player, std::vector<Brick> &bricks);

Color getRandomColor();

int main()
{

    InitWindow(g_screenWidth, g_screenHeight, g_gameTitle);

    SetTargetFPS(60);

    // Player related
    constexpr Color playerColor{RED};

    // Initializing the player
    // Setting the initial position
    // Setting width and height
    Rectangle player{
        g_screenWidth / 2.0f - 50, // x: top-left corner position x
        g_screenHeight - 25,       // y: top-left corner position y
        100,                       // width
        25};                       // height

    // Initializing the ball
    // Setting the initial position
    // Setting width and height
    Ball ball{
        {g_screenWidth / 2.0f, // x: center position x
         player.y - 25},       // y: center position y
        20,                    // radius
        BLUE,                  // color
    };

    // Store all the bricks in this vector
    // and set their positions for each Brick object
    std::vector<Brick> bricks{};

    for (short i{0}; i < g_fitScreenHeight; ++i)
    {
        for (short j{0}; j < g_fitScreenWidth; ++j)
        {
            bricks.push_back(Brick{
                {
                    static_cast<float>(j * g_boxSize),
                    static_cast<float>(i * g_boxSize),
                    g_boxSize,
                    g_boxSize,
                },
                getRandomColor(),
            });
        }
    }

    while (!WindowShouldClose())
    {

        // Update player's position based on user's input
        updatePlayerPosition(player);

        // Check ball's collision with player, walls, and bricks
        checkBallCollision(ball, player, bricks);

        // If the ball goes beyond the bottom edge, exit the game
        if ((ball.position.y + ball.radius) > g_screenHeight)
            break;

        BeginDrawing();

        ClearBackground(RAYWHITE);

        // Draw all the bricks in the given vector
        for (Brick brick : bricks)
            DrawRectangleRec(brick.brick, brick.color);

        // Draw the current state of the player
        // Rectangles are drawn from the
        // top-left corner both x and y
        DrawRectangleRec(player, playerColor);

        // Draw the current state of the Ball
        // Circle are drawn from the
        // center both x and y
        DrawCircleV(ball.position, ball.radius, ball.color);

        DrawFPS(0, 0);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}

void updatePlayerPosition(Rectangle &player)
{

    constexpr short playerSpeed{10};

    // check if the player is trying to move beyond
    // the left or right edge of the screen.

    if (IsKeyDown(KEY_RIGHT) && (player.x + player.width) < g_screenWidth)
        player.x += playerSpeed;
    else if (IsKeyDown(KEY_LEFT) && player.x > 0)
        player.x -= playerSpeed;
}

void checkBallCollision(Ball &ball, const Rectangle &player, std::vector<Brick> &bricks)
{

    // Ball related
    constexpr short ballSpeed{5};

    // Keep the track of the direction of the ball
    // beyond the function boundary
    static short xDirection{1};
    static short yDirection{1};

    // If the ball reaches the left edge of the screen bounce it back
    if ((ball.position.x + ball.radius) == g_screenWidth)
        xDirection = -1;

    // If the ball reaches the right edge of the screen bounce it back
    else if ((ball.position.x - ball.radius) == 0)
        xDirection = 1;

    // If the ball reaches the ground bounce it back
    // if ((ball.position.y + ball.radius) == g_screenHeight)
    //     yDirection = -1;

    // If the ball reaches the ceiling bounce it back
    if ((ball.position.y - ball.radius) == 0)
        yDirection = 1;

    // If the ball collided with the Player
    if (CheckCollisionCircleRec(ball.position, ball.radius, player))
        yDirection = -1;

    // If the ball collided with any bricks
    for (std::vector<Brick>::iterator i = bricks.begin(); i != bricks.end(); ++i)
    {
        if (i->checkCollisionWithBall(ball))
        {
            yDirection = 1;
            bricks.erase(i); // If collided remove that brick
        }
    }

    ball.position.x += ballSpeed * xDirection;
    ball.position.y += ballSpeed * yDirection;
}

Color getRandomColor()
{
    return Color{
        static_cast<unsigned char>(GetRandomValue(0, 255)),
        static_cast<unsigned char>(GetRandomValue(0, 255)),
        static_cast<unsigned char>(GetRandomValue(0, 255)),
        static_cast<unsigned char>(GetRandomValue(0, 255)),
    };
}