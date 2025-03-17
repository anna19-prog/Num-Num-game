#include "raylib.h"
#include <iostream>
#include <vector>
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
    long long framesCounter = 0;

    typedef enum GameScreen { RULES, MAC, SURF, POKRA, EXIT } GameScreen;
    InitWindow(screenWidth, screenHeight, "kto kak rabotaet, tot tak i est"); // минута философии
    SetTargetFPS(60);
    GameScreen currentScreen = RULES;


	Texture2D IlyaPicture = LoadTexture("src/pictures/photoIlya.jpg");
	Texture2D BurgerPicture = LoadTexture("src/pictures/Burger.png");

    Image background1 = LoadImage("src/pictures/mac.png");
    ImageResize(&background1, screenWidth, screenHeight);
    Texture2D background = LoadTextureFromImage(background1);

    int score = 0;
    float iluhaX = screenWidth / 2 - playerWidth / 2;
    const float playerSpeed = 300.0f;
    
    std::vector<FallingObject> objects;
    FallingObject b = CreateFallingObject(BurgerPicture); //Тут бургер уже успешно создан
    objects.push_back(b);
    
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();  // Время кадра (нужно для плавного движения)
        BeginDrawing();
        ClearBackground(RAYWHITE);

        framesCounter = (framesCounter + 1) % 1000;
        switch (currentScreen) 
        {
            case RULES:
            {
                if (framesCounter > 300)
                {
                    currentScreen = MAC;
                }
                DrawText("MEOW MEOW THIS IS THE RULES", 20, 20, 40, LIGHTGRAY);
            } break;
            case MAC: // мы в маке
            {
                if (IsKeyDown(KEY_LEFT) && iluhaX > 0) {
                    iluhaX -= playerSpeed * dt;
                }
                
                else if (IsKeyDown(KEY_RIGHT) && iluhaX < screenWidth - playerWidth) {
                    iluhaX += playerSpeed * dt;
                }
                    
        
                for (auto& burger: objects) { // обновляем все падающие бургеры
                    burger.y += burger.speed * dt;
        
                    if ((burger.y + fallingHeight >= screenHeight - playerHeight) &&
                    (burger.x + fallingWidth >= iluhaX) &&
                    (burger.x <= iluhaX + playerWidth)) {
                        score++;
                        FallingObject temp_burger = CreateFallingObject(BurgerPicture);
                        objects.push_back(temp_burger);
                    }
        
                    // Если объект улетел вниз — создаём новый
                    if (burger.y > screenHeight) {
                        FallingObject temp_burger = CreateFallingObject(BurgerPicture);
                        objects.push_back(temp_burger);
                    }
                }

                if (score == 10) {
                    currentScreen = SURF;
                }

                DrawTexture(background, 0, 0, WHITE);
                DrawText(TextFormat("Score: %d", score), 20, 20, 20, DARKGRAY);
                DrawTexture(IlyaPicture, iluhaX, screenHeight - playerHeight, WHITE);
                for (auto& burger: objects) DrawTexture(burger.picture, burger.x, burger.y, WHITE);

            } break;
            case SURF: //мы в сёрфе
            {
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
                {
                    currentScreen = POKRA;
                }
                DrawRectangle(0, 0, screenWidth, screenHeight, PURPLE);
                DrawText("GAMEPLAY SCREEN", 20, 20, 40, MAROON);
                DrawText("PRESS ENTER or TAP to JUMP to ENDING SCREEN", 130, 220, 20, MAROON);
            } break;
            case POKRA: // мы на покре
            {
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
                    currentScreen = EXIT;
                }
                DrawRectangle(0, 0, screenWidth, screenHeight, BLUE);
                DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
                DrawText("PRESS ENTER or TAP to RETURN to TITLE SCREEN", 120, 220, 20, DARKBLUE);

            } break;
            case EXIT: //гейм овер
            {
                if (framesCounter % 500 == 0) {
                    break;
                }
                DrawText("GAME OVER", 20, 20, 40, DARKGREEN);
            }
            default: break;
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
