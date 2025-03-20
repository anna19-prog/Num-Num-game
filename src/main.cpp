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

    typedef enum GameScreen { BEGINNING, RULES, MAC, GROWING, SURF, SPEEDING, POKRA, EXIT } GameScreen;
    InitWindow(screenWidth, screenHeight, "kto kak rabotaet, tot tak i est"); // минута философии
    GameScreen currentScreen = BEGINNING;
    SetTargetFPS(60);
    long long framesCounter = 0;

    int playerWidth = 100;
    int playerHeight = 100;
    int objectsWidth = 60;
    int objectsHeight = 60;
    Texture2D IlyaPicture = get_pic(playerWidth, playerHeight, "src/pictures/photoIlya.jpg"); // картинка персонажа
    Texture2D WinPicture = get_pic(screenWidth / 2, screenHeight / 2, "src/pictures/meoow.png");
    Texture2D BurgerPicture = get_pic(objectsWidth, objectsHeight, "src/pictures/Burger.png"); // картинка бургера
    Texture2D CoffeePicture = get_pic(objectsWidth, objectsHeight, "src/pictures/coffee_cup.png"); // картинка сёрф кофе
    Texture2D SaladPicture = get_pic(objectsWidth, objectsHeight, "src/pictures/salad.png"); // картинка салата
    Texture2D ComputerPicture = get_pic(objectsWidth, objectsHeight, "src/pictures/computer.png"); // комп
    Texture2D background_mac = get_pic(screenWidth, screenHeight, "src/pictures/mac.png"); // картинка мак
    Texture2D background_surf = get_pic(screenWidth, screenHeight, "src/pictures/surf.png"); // картинка сёрф
    Texture2D background_pokra = get_pic(screenWidth, screenHeight, "src/pictures/pokra.jpg"); // картинка покра

    Player Ilya(300.0f, playerWidth, playerHeight, (Vector2){static_cast<float>(screenWidth / 2 - playerWidth / 2), 
        static_cast<float>(screenHeight - playerHeight)}, IlyaPicture, 0.0f);

    int score = 0;
    std::vector<int> wanted_scores{15, 25, 30}; // скоры для перехода на следующий уровень
    bool won = true;
    
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
                    currentScreen = RULES;
                    framesCounter = 0;
                }
                DrawText("ILYA NUM-NUM GAME \n IT IS NOT FOR THE WEAK\n\n\npress enter to begin", 20, 20, 40, GRAY);
            } break;
            case RULES:
            {
                framesCounter++;
                if (framesCounter > 20 && IsKeyDown(KEY_ENTER)) {
                    currentScreen = MAC;
                }
                DrawText("your goal is to catch the falling food and drinks\n\nbe careful with what\n you choose to eat", 20, 100, 40, GRAY);
            } break;
            case MAC: // мы в маке
            {
                framesCounter++;
                Ilya.update(dt);
        
                for (auto& burger: objects) { // обновляем все падающие бургеры
                    burger.update(dt, Ilya, score);
                }

                if (framesCounter > 90) {
                    framesCounter = 0;
                    if (score <= 7 || rand() % 2 == 0) { // рандомно выбираем -- салад или бургер
                        Food temp(objectsWidth, objectsHeight, BurgerPicture, 1);
                        objects.push_back(temp);
                    } else {
                        Food temp(objectsWidth, objectsHeight, SaladPicture, 0);
                        objects.push_back(temp);
                    }
                }

                if (score == wanted_scores[0]) {
                    framesCounter = 0;
                    currentScreen = GROWING;
                }

                if (score < 0) {
                    framesCounter = 0;
                    currentScreen = EXIT;
                    won = false;
                }

                DrawTexture(background_mac, 0, 0, WHITE);
                if (Ilya.ate_smth_bad) {
                    DrawText(TextFormat("Score: %d", score), 20, 20, 40, RED);
                } else {
                    DrawText(TextFormat("Score: %d", score), 20, 20, 30, WHITE);
                }
                DrawTexture(IlyaPicture, Ilya.pos.x, screenHeight - Ilya.height, WHITE);
                for (auto& burger: objects) if (burger.active) DrawTexture(burger.picture, burger.pos.x, burger.pos.y, WHITE);
            } break;
            case GROWING:
            {   
                framesCounter++;
                DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
                Ilya.growing();
                DrawText("HE'S GROWING", 20, 20, 50, DARKGRAY);
                
                if (framesCounter > 600) {
                    currentScreen = SURF;
                    Ilya.pos = (Vector2){static_cast<float>(screenWidth / 2 - playerWidth / 2), 
                        static_cast<float>(screenHeight - playerHeight)};
                    objects.clear();
                    Ilya.speed = 150.0f;
                    Ilya.accelaration = 50.0f;
                    framesCounter = 0;
                }
            } break;
            case SURF: // мы в сёрфе
            {
                framesCounter++;
                Ilya.update(dt);

                for (auto& coffee: objects) { 
                    coffee.update(dt, Ilya, score);
                }

                if (framesCounter > 90) {
                    framesCounter = 0;
                    Food temp_coffee(objectsWidth, objectsHeight, CoffeePicture, 1);
                    objects.push_back(temp_coffee);
                }

                if (score == wanted_scores[1]) {
                    framesCounter = 0;
                    currentScreen = SPEEDING;
                    Ilya.pos = (Vector2){static_cast<float>(-playerWidth - 10), screenHeight / 2};
                    objects.clear();
                    Ilya.speed = 150.0f;
                }

                if (score < 0) {
                    framesCounter = 0;
                    currentScreen = EXIT;
                    won = false;
                }

                DrawTexture(background_surf, 0, 0, WHITE);
                DrawText(TextFormat("Score: %d", score), 20, 20, 30, WHITE);
                DrawTexture(Ilya.picture, Ilya.pos.x, screenHeight - Ilya.height, WHITE);
                for (auto& coffee: objects) if (coffee.active) DrawTexture(coffee.picture, coffee.pos.x, coffee.pos.y, WHITE);

                
            } break;
            case SPEEDING:
            {
                framesCounter++;
                DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
                DrawText("HE'S SPEEDING", 20, 20, 50, DARKGRAY);
                Ilya.speeding(dt);
                

                if (Ilya.pos.x > screenWidth) {
                    currentScreen = POKRA;
                    Ilya.pos = (Vector2){static_cast<float>(screenWidth / 2 - playerWidth / 2), 
                        static_cast<float>(screenHeight - playerHeight)};
                    Ilya.speed = 350.0f;
                    Ilya.accelaration = 0.0f;
                    framesCounter = 0;
                }

            } break;
            case POKRA: // мы на покре
            {
                framesCounter++;
                Ilya.update(dt);

                for (auto& comp: objects) { // обновляем все падающие бургеры
                    comp.update(dt, Ilya, score);
                }

                if (framesCounter > 90) {
                    framesCounter = 0;
                    Food temp_coffee(objectsWidth, objectsHeight, ComputerPicture, 1);
                    objects.push_back(temp_coffee);
                }

                if (score == wanted_scores[2]) {
                    framesCounter = 0;
                    currentScreen = EXIT;
                }

                if (score < 0) {
                    framesCounter = 0;
                    currentScreen = EXIT;
                    won = false;
                }

                DrawTexture(background_pokra, 0, 0, WHITE);
                if (Ilya.ate_smth_bad) {
                    DrawText(TextFormat("Score: %d", score), 20, 20, 30, RED);
                } else {
                    DrawText(TextFormat("Score: %d", score), 20, 20, 30, BLACK);
                }
                DrawText(TextFormat("Score: %d", score), 20, 20, 30, BLACK);
                DrawTexture(IlyaPicture, Ilya.pos.x, screenHeight - Ilya.height, WHITE);
                for (auto& coffee: objects) if (coffee.active) DrawTexture(coffee.picture, coffee.pos.x, coffee.pos.y, WHITE);


            } break;
            case EXIT: //гейм овер
            {
                framesCounter++;
                if (framesCounter > 300) {
                    running = false;
                }
                DrawText("GAME OVER", 20, 20, 40, DARKGREEN);
                if (won) {
                    DrawText("YOU DID IT!!", 20, 80, 50, BLUE);
                    DrawTexture(WinPicture, screenWidth / 2, screenHeight / 2, WHITE);
                } else {
                    DrawText("YOU LOST", 20, 80, 50, RED);
                }
            }
            default: break;
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
