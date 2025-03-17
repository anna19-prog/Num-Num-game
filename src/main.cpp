#include "raylib.h"
#include <iostream>
#include <ctime> //используем эту библиотеку так как встроенная функция rand выдает псевдослучайные числа,
                // которые на самом деле они и те же (мы же устанавливаем нулевое стартовое число и каждай раз получаем разный рандом srand(time(0));)
const int screenWidth = 800;
const int screenHeight = 600;
const int playerWidth = 100;
const int playerHeight = 100;
const int fallingWidth = 60;
const int fallingHeight = 60;

struct FallingObject {
    float x;
	float y;
    float speed;
    Texture2D picture;
};

FallingObject CreateFallingObject(Texture2D pic) {
    FallingObject obj;
    obj.x = rand() % (screenWidth - fallingWidth); // мудрая техника вычисления рандома в опр диапазоне
    obj.y = 0;
    obj.speed = rand() % 100 + 200.0f;
    obj.picture = pic;
    return obj;
}

int main() {
    srand(time(0));
    InitWindow(screenWidth, screenHeight, "kto kak rabotaet, tot tak i est");
    SetTargetFPS(60);

	Texture2D IlyaPicture = LoadTexture("src/pictures/photoIlya.jpg");
	Texture2D BurgerPicture = LoadTexture("src/pictures/Burger.png");
    Texture2D background = LoadTexture("src/pictures/mac.png");
    //ImageResize(background, screenWidth, screenHeight);

    int score = 0;
    float iluhaX = screenWidth / 2 - playerWidth / 2;
    const float playerSpeed = 300.0f;

    std::vector<FallingObject> objects;
    //FallingObject burger = CreateFallingObject(BurgerPicture); //Тут бургер уже успешно создан
    objects.push_back(CreateFallingObject(BurgerPicture));

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();  // Время кадра (нужно для плавного движения)
        if (IsKeyDown(KEY_LEFT) && iluhaX > 0)
            iluhaX -= playerSpeed * dt;

        if (IsKeyDown(KEY_RIGHT) && iluhaX < screenWidth - playerWidth)
            iluhaX += playerSpeed * dt;

        for (FallingObject burger: objects) { // обновляем все падающие бургеры
            burger.y += burger.speed * dt;

            if ((burger.y + fallingHeight >= screenHeight - playerHeight) &&
            (burger.x + fallingWidth >= iluhaX) &&
            (burger.x <= iluhaX + playerWidth)) {
                score++;
                burger = CreateFallingObject(BurgerPicture);
            }

            // Если объект улетел вниз — создаём новый
            if (burger.y > screenHeight) {
                burger = CreateFallingObject(BurgerPicture);
            }
        }

        
        BeginDrawing();
        ClearBackground({255, 182, 193, 255});
        DrawTexture(background, 0, 0, WHITE);
        DrawText(TextFormat("Score: %d", score), 20, 20, 20, DARKGRAY);
        DrawTexture(IlyaPicture, iluhaX, screenHeight - playerHeight, WHITE);
        for (FallingObject burger: objects) DrawTexture(burger.picture, burger.x, burger.y, WHITE);
        EndDrawing();
    }

    CloseWindow();
}
