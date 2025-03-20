#include "raylib.h"
#include <vector>


const int screenWidth = 800;
const int screenHeight = 600;


class Object
{
public:
    float speed;
	int width;
    int height;
    Vector2 pos;
    Texture2D picture;

    Object(int w, int h, Texture2D pic) : width(w), height(h), picture(pic) {}

	void update(float delta_time);
};

class Player : public Object
{
public:
    float scale = 0.01;
    bool ate_smth_bad = 0;
    float accelaration; // ускорение которое будет придавать кофе

    Player(float s, int w, int h, Vector2 p, Texture pic, float acc);
	void update(float delta_time);
    void growing();
    void speeding(float delta_time);
};

class Food : public Object
{
public:
    bool active;
    bool good;
    Sound coin = LoadSound("src/sounds/coin.wav");

    Food(int w, int h, Texture2D pic, bool g);
    void update(float delta_time, Player& Ilya, int& score);
};