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
    bool active = 1;
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

    typedef enum GameScreen { RULES, MAC, GROWING, SURF, POKRA, EXIT } GameScreen;
    InitWindow(screenWidth, screenHeight, "kto kak rabotaet, tot tak i est"); // минута философии
    GameScreen currentScreen = RULES;
    SetTargetFPS(60);
    long long framesCounter = 0;

	Texture2D IlyaPicture = LoadTexture("src/pictures/photoIlya.jpg");
	Texture2D BurgerPicture = LoadTexture("src/pictures/Burger.png");

    Image background1 = LoadImage("src/pictures/mac.png");
    ImageResize(&background1, screenWidth, screenHeight);
    Texture2D background = LoadTextureFromImage(background1);

    int score = 0;
    float iluhaX = screenWidth / 2 - playerWidth / 2;
    const float playerSpeed = 300.0f;
    float scale = 0.01; // чтобы скейлить картинку когда растём
    
    std::vector<FallingObject> objects;
    //FallingObject b = CreateFallingObject(BurgerPicture); //Тут бургер уже успешно создан
    //objects.push_back(b);
    
    bool running = true;
    while (running) {
        float dt = GetFrameTime();  // Время кадра (нужно для плавного движения)
        BeginDrawing();
        ClearBackground(RAYWHITE);
        switch (currentScreen) 
        {
            case RULES:
            {
                framesCounter++;
                if (framesCounter > 300)
                {
                    framesCounter = 0;
                    currentScreen = MAC;
                }
                DrawText("ILYA NUM-NUM GAME \n IT IS NOT FOR THE WEAK", 20, 20, 40, LIGHTGRAY);
            } break;
            case MAC: // мы в маке
            {
                framesCounter++;
                if (IsKeyDown(KEY_LEFT) && iluhaX > 0) {
                    iluhaX -= playerSpeed * dt;
                }
                
                else if (IsKeyDown(KEY_RIGHT) && iluhaX < screenWidth - playerWidth) {
                    iluhaX += playerSpeed * dt;
                }   
        
                for (auto& burger: objects) { // обновляем все падающие бургеры
                    if (burger.active) {
                        burger.y += burger.speed * dt;
        
                        if ((burger.y + fallingHeight >= screenHeight - playerHeight) &&
                        (burger.x + fallingWidth >= iluhaX) &&
                        (burger.x <= iluhaX + playerWidth)) {
                            score++;
                            burger.active = 0; 
                        }
            
                        // Если объект улетел вниз — создаём новый
                        // if (burger.y > screenHeight) {
                        //     FallingObject temp_burger = CreateFallingObject(BurgerPicture);
                        //     objects.push_back(temp_burger);
                        // }
                    }
                }

                if (framesCounter > 90) {
                    framesCounter = 0;
                    FallingObject temp_burger = CreateFallingObject(BurgerPicture);
                    objects.push_back(temp_burger);
                }

                if (score == 10) {
                    framesCounter = 0;
                    currentScreen = GROWING;
                }

                DrawTexture(background, 0, 0, WHITE);
                DrawText(TextFormat("Score: %d", score), 20, 20, 30, WHITE);
                DrawTexture(IlyaPicture, iluhaX, screenHeight - playerHeight, WHITE);
                for (auto& burger: objects) if (burger.active) DrawTexture(burger.picture, burger.x, burger.y, WHITE);

            } break;
            case GROWING:
            {   
                framesCounter++;
                DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
                DrawText("HE'S GROWING", 20, 20, 50, DARKGRAY);
                DrawTextureEx(IlyaPicture, (Vector2){screenWidth / 4, screenHeight / 4}, 0.0f, scale, WHITE);
                if (framesCounter > 600) {
                    currentScreen = SURF;
                    framesCounter = 0;
                }
                scale += 0.01;
            } break;
            case SURF: // мы в сёрфе
            {
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
                {
                    currentScreen = POKRA;
                }
                DrawRectangle(0, 0, screenWidth, screenHeight, PURPLE);
                DrawText("GAMEPLAY SCREEN", 20, 20, 40, MAROON);
                DrawText("PRESS ENTER or TAP to JUMP to ENDING SCREEN", 130, 220, 20, MAROON);
                framesCounter = 0;
            } break;
            case POKRA: // мы на покре
            {
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
                    currentScreen = EXIT;
                }
                DrawRectangle(0, 0, screenWidth, screenHeight, BLUE);
                DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
                DrawText("PRESS ENTER or TAP to RETURN to TITLE SCREEN", 120, 220, 20, DARKBLUE);
                framesCounter = 0;

            } break;
            case EXIT: //гейм овер
            {
                framesCounter++;
                if (framesCounter > 300) {
                    running = false;
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
