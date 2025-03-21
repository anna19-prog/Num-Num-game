#include "game.hpp"


Player::Player(float s, int w, int h, Vector2 p, Texture pic, float a) : Object(w, h, pic) 
{
    speed = s;
    width = w;
    height = h;
    pos = p;
    picture = pic;
    accelaration = a;
}

void Player::update(float delta_time)
{
    if (IsKeyDown(KEY_LEFT) && pos.x > 0) {
        pos.x -= speed * delta_time;
    }
    
    else if (IsKeyDown(KEY_RIGHT) && pos.x < screenWidth - width) {
        pos.x += speed * delta_time;
    }
}

 void Player::growing() {
     DrawTextureEx(picture, (Vector2){screenWidth / 2 - width * scale / 2, screenHeight / 2 - height * scale / 2}, 0.0f, scale, WHITE);
     scale += 0.03;
 }

 void Player::speeding(float delta_time) {
    pos.x += speed * delta_time;
    speed += accelaration;
    DrawTexture(picture, pos.x, screenHeight / 2, WHITE);
 }

 Food::Food(int w, int h, Texture2D pic, bool g) : Object(w, h, pic) 
 {
    speed = GetRandomValue(0, 100) + 200.0f;
    width = w;
    height = h;
    pos = (Vector2){static_cast<float>(rand() % (screenWidth - width)), 0};
    picture = pic;
    active = 1;
    good = g;
 }

 void Food::update(float delta_time, Player& Ilya, int& score) {
    if (active) {
        pos.y += speed * delta_time;

        if ((pos.y + height >= screenHeight - Ilya.height) &&
        (pos.x + width >= Ilya.pos.x) &&
        (pos.x <= Ilya.pos.x + Ilya.width)) {
            if (good) {
                score++;
                Ilya.ate_smth_bad = 0;
                PlaySound(coin);
            } else {
                score -= 2;
                Ilya.ate_smth_bad = 1;
            }
            
            active = 0;
            Ilya.speed += Ilya.accelaration; 
        }

        if (pos.y > screenHeight && active) {
            if (good) {
                score--;
                Ilya.ate_smth_bad = 1;
            }
            active = 0;
        }
    }
 }

