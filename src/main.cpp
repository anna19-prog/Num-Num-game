#include "raylib.h"
#include <iostream>
#include <vector>
#include <stdlib.h> 

using std::vector;
using std::rand;

#define screenWidth  800
#define screenHeight  450

struct Ball { // скорее всего надо переделать в класс..
	Vector2 ballPosition = { 200.0f, 0.0f }; //static_cast<float>(rand()) % (screenWidth - 20)
	float ballSpeed = 2.0f ;
	int ballRadius = 10;
	bool active = 1; // жив или нет
};


int main(void)
{
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "meow meow meow");

    //Vector2 ballPosition = { GetScreenWidth()/2.0f, GetScreenHeight() };
    //Vector2 ballSpeed = { 2.0f, 2.0f };
    //int ballRadius = 20; // перенесла инициализацию мячиков в структуру
	vector<Ball> balls; // список мячиков
	Ball ball1;
	balls.push_back(ball1);

    bool pause = 0;
    int framesCounter = 0;

    SetTargetFPS(60);              

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        if (IsKeyPressed(KEY_SPACE)) pause = !pause;

        if (!pause)
        {
            for (int i = 0; i < balls.size(); ++i) {
				balls[i].ballPosition.y += balls[i].ballSpeed;
				if (balls[i].ballPosition.y >= (screenHeight - balls[i].ballRadius)) { // если до пола долетел
					balls[i].ballSpeed = 0;
					balls[i].active = 0;
			}
			}
        }
        else framesCounter++;
		if (framesCounter % 60 == 0) {
			Ball new_ball { {static_cast<float>(rand() % (screenWidth - 10) + 10), 0.0f}, 2.0f, 10, 1};
			balls.push_back(new_ball);
		}

        // Draw
        //-----------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

			for (int i = 0; i < balls.size(); ++i) {
				if (balls[i].active) DrawCircleV(balls[i].ballPosition, (float)balls[i].ballRadius, MAROON);
			}

			// если пауза -- мигающее сообщение
			DrawText("PRESS SPACE to PAUSE BALL MOVEMENT", 10, screenHeight - 25, 20, LIGHTGRAY); // может пригодиться
			if (pause && ((framesCounter/30)%2)) DrawText("PAUSED", 350, 200, 30, GRAY);

            DrawFPS(10, 10);

        EndDrawing();
    }
    CloseWindow();        // Close window and OpenGL context
    return 0;
}