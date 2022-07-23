#include "main.h"
#include "raylib.h"
#include "linked_list.h"


const int scale = 18;
const int gridWidth = 30;
const int gridHeight = 30;
const int screenWidth = gridWidth*scale;
const int screenHeight = gridHeight*scale;
const int appleTickLimit = (gridWidth+gridHeight)*0.8;
const float startingSpeed = 0.25f;
const float speedRate = 0.005f;
const float speedLimit = 0.060f;
const int scoreMultiplier = 10;

Vector2 origin;
Rectangle rect;

int rotation;

enum Direction {
    None,
    Up,
    Down,
    Left,
    Right
};

struct Apple {
    int x;
    int y;
    int ticker;
} apple;

struct Snake {
    struct Node* head;
    struct Node* lastNode;
    enum Direction direction;
    enum Direction nextDirection;
    int length;
    float timer;
    float tickRate;
    bool isDead;
} snake;

int main(void) {

    InitWindow(screenWidth, screenHeight, "Snake");
    SetTargetFPS(60);
    Setup();

    while (!WindowShouldClose())
    {
        Update();
        BeginDrawing();
        ClearBackground(BLACK);
        Draw();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

void Setup() {
    snake.head = NodeNew((gridWidth/2)*scale, (gridHeight/2)*scale);
    snake.head->next = NodeNew(((gridWidth/2)-1)*scale, ((gridHeight/2))*scale);
    snake.length = 2;
    snake.lastNode = snake.head->next;
    snake.direction = Right;
    snake.nextDirection = Right;
    snake.timer = 0.0f;
    snake.tickRate = startingSpeed;
    snake.isDead = false;

    apple.ticker = 0;
    apple.x = GetRandomValue(1, gridWidth-1)*scale;
    apple.y = GetRandomValue(1, gridHeight-1)*scale;

    origin.x = 0;
    origin.y = 0;

    rect.height = scale;
    rect.width = scale;

    rotation = 0;
}

void Update() {

    if (IsKeyPressed(KEY_R)) {
        NodeDeleteSelfAndTail(snake.head);
        Setup();
        return;
    }

    float dt = GetFrameTime();

    if (snake.isDead) {
        rotation += 1;
        return;
    }

    if (IsKeyDown(KEY_D) && snake.direction != Left) snake.nextDirection = Right;
    if (IsKeyDown(KEY_A) && snake.direction != Right) snake.nextDirection = Left;
    if (IsKeyDown(KEY_W) && snake.direction != Down) snake.nextDirection = Up;
    if (IsKeyDown(KEY_S) && snake.direction != Up) snake.nextDirection = Down;
    
    snake.timer += dt;
    if (snake.timer < snake.tickRate) return;

    snake.direction = snake.nextDirection;

    if (snake.head->x == apple.x && snake.head->y == apple.y) {
        snake.lastNode->next = NodeNew(snake.lastNode->x, snake.lastNode->y);
        snake.lastNode = snake.lastNode->next;
        ++snake.length;
        apple.x = GetRandomValue(1, gridWidth-1)*scale;
        apple.y = GetRandomValue(1, gridHeight-1)*scale;
        apple.ticker = 0;
        if (snake.tickRate > speedLimit) snake.tickRate -= speedRate;
    }

    ++apple.ticker;
    if (apple.ticker >= appleTickLimit) {
        apple.x = GetRandomValue(1, gridWidth-1)*scale;
        apple.y = GetRandomValue(1, gridHeight-1)*scale;
        apple.ticker = 0;
    }

    snake.timer = 0.0f;
    int prevX = snake.head->x;
    int prevY = snake.head->y;
    int holdX = snake.head->x;
    int holdY = snake.head->y;
    struct Node* node = snake.head->next;

    switch (snake.direction) {
    case Up:
        snake.head->y -= scale;
        break;
    case Down:
        snake.head->y += scale;
        break;
    case Left:
        snake.head->x -= scale;
        break;
    case Right:
        snake.head->x += scale;
        break;
    case None:
        break;
    }

    if (snake.head->x > (gridWidth-1)*scale) snake.head->x = 0;
    if (snake.head->x < 0) snake.head->x = (gridWidth-1)*scale;
    if (snake.head->y > (gridHeight-1)*scale) snake.head->y = scale;
    if (snake.head->y < scale) snake.head->y = (gridHeight-1)*scale;
    
    while (node != NULL) {
        if (snake.head->x == node->x && snake.head->y == node->y) {
            snake.isDead = true;
        }
        holdX = node->x;
        holdY = node->y;
        node->x = prevX;
        node->y = prevY;
        prevX = holdX;
        prevY = holdY;
        node = node->next;
    }
}

void Draw() {
    if (!snake.isDead) DrawRectangle(apple.x, apple.y, scale, scale, WHITE);
    struct Node* next = snake.head->next;
    while (next != NULL) {
        rect.x = next->x;
        rect.y = next->y;
        DrawRectanglePro(rect, origin, rotation, BROWN);
        next = next->next;
    }
    rect.x = snake.head->x;
    rect.y = snake.head->y;
    DrawRectanglePro(rect, origin, rotation, ORANGE);

    DrawRectangle(0, 0, 30*scale, scale, PURPLE);
    DrawText(TextFormat("Score: %i", (snake.length-2)*scoreMultiplier), scale/2, 1, scale, WHITE);

    if (snake.isDead) {
        DrawText(TextFormat("Final Score: %i", (snake.length-2)*scoreMultiplier), scale/2, scale*2, scale*2, WHITE);
    }
}
