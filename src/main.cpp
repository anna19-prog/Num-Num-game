#include "raylib.h"
#include <iostream>
#include <string>
#include <vector>
#include <ctime> //используем эту библиотеку так как встроенная функция rand выдает псевдослучайные числа,
                // которые на самом деле они и те же (мы же устанавливаем нулевое стартовое число и каждай раз получаем разный рандом srand(time(0));)

#include <game.hpp>


Texture2D get_pic(int width, int height, const char* path_) { // загрузка картинок
    Image temp = LoadImage(path_);
    ImageResize(&temp, width, height);
    Texture2D picture = LoadTextureFromImage(temp);
    return picture;
}

int main() {
    srand(time(0));

    typedef enum GameScreen { BEGINNING, RULES, MAC, GROWING, SURF, POKRA, EXIT } GameScreen;
    InitWindow(screenWidth, screenHeight, "kto kak rabotaet, tot tak i est"); // минута философии
    GameScreen currentScreen = BEGINNING;
    SetTargetFPS(60);
    long long framesCounter = 0;

    int playerWidth = 100;
    int playerHeight = 100;
    Texture2D IlyaPicture = get_pic(playerWidth, playerHeight, "src/pictures/photoIlya.jpg"); // картинка персонажа
    Texture2D BurgerPicture = get_pic(60, 60, "src/pictures/Burger.png"); // картинка бургера
    Texture2D CoffeePicture = get_pic(60, 60, "src/pictures/coffee_cup.png"); // картинка сёрф кофе
    Texture2D background_mac = get_pic(screenWidth, screenHeight, "src/pictures/mac.png"); // картинка мак
    Texture2D background_surf = get_pic(screenWidth, screenHeight, "src/pictures/surf.png"); // картинка сёрф

    Player Ilya(300.0f, playerWidth, playerHeight, (Vector2){static_cast<float>(screenWidth / 2 - playerWidth / 2), 
        static_cast<float>(screenHeight - playerHeight)}, IlyaPicture);

    int score = 0;
    
    std::vector<Food> objects; // все наши падающие объекты
    
    bool running = true;
    while (running && !WindowShouldClose()) {
        float dt = GetFrameTime();  // Время кадра (нужно для плавного движения)
        BeginDrawing();
        ClearBackground(RAYWHITE);
        switch (currentScreen) 
        {
            case BEGINNING:
            {
                if (IsKeyDown(KEY_ENTER)) {
                    currentScreen = MAC;
                }
                DrawText("ILYA NUM-NUM GAME \n IT IS NOT FOR THE WEAK\n\neat burgers in order for Ilya to grow\n\n\npress enter to begin", 20, 20, 40, GRAY);
            } break;
            case RULES:
            {
                if (IsKeyDown(KEY_ENTER)) {
                    currentScreen = MAC;
                }
                DrawText("your goal is to catch the falling food and drinks", 20, 20, 40, GRAY);
            }break;
            case MAC: // мы в маке
            {
                framesCounter++;
                Ilya.update(dt);
        
                for (auto& burger: objects) { // обновляем все падающие бургеры
                    burger.update(dt, Ilya, score);
                }

                if (framesCounter > 90) {
                    framesCounter = 0;
                    Food temp_burger(60, 60, BurgerPicture, 0);
                    objects.push_back(temp_burger);
                }

                if (score == 10) {
                    framesCounter = 0;
                    currentScreen = GROWING;
                }

                if (score < 0) {
                    framesCounter = 0;
                    currentScreen = EXIT;
                }

                DrawTexture(background_mac, 0, 0, WHITE);
                DrawText(TextFormat("Score: %d", score), 20, 20, 30, WHITE);
                DrawTexture(IlyaPicture, Ilya.pos.x, screenHeight - Ilya.height, WHITE);
                for (auto& burger: objects) if (burger.active) DrawTexture(burger.picture, burger.pos.x, burger.pos.y, WHITE);
            } break;
            case GROWING:
            {   
                framesCounter++;
                DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
                DrawText("HE'S GROWING", 20, 20, 50, DARKGRAY);
                Ilya.growing();
                
                if (framesCounter > 600) {
                    currentScreen = SURF;
                    Ilya.pos = (Vector2){static_cast<float>(screenWidth / 2 - playerWidth / 2), 
                        static_cast<float>(screenHeight - playerHeight)};
                    objects.clear();
                    Ilya.speed = 150.0f;
                    framesCounter = 0;
                }
            } break;
            case SURF: // мы в сёрфе
            {
                framesCounter++;
                Ilya.update(dt);

                for (auto& coffee: objects) { // обновляем все падающие бургеры
                    coffee.update(dt, Ilya, score);
                }

                if (framesCounter > 90) {
                    framesCounter = 0;
                    Food temp_coffee(60, 60, CoffeePicture, 50.0f);
                    objects.push_back(temp_coffee);
                }

                if (score == 25) {
                    framesCounter = 0;
                    currentScreen = POKRA;
                }

                if (score < 0) {
                    framesCounter = 0;
                    currentScreen = EXIT;
                }

                DrawTexture(background_surf, 0, 0, WHITE);
                DrawText(TextFormat("Score: %d", score), 20, 20, 30, WHITE);
                DrawTexture(IlyaPicture, Ilya.pos.x, screenHeight - Ilya.height, WHITE);
                for (auto& coffee: objects) if (coffee.active) DrawTexture(coffee.picture, coffee.pos.x, coffee.pos.y, WHITE);

                
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
