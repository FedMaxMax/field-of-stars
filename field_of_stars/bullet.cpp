#include "bullet.h"

Bullet::Bullet(Image &image, float X, float Y, int W, int H, std::string Name, StateObject State)
    :Entity(image, X, Y, W, H, Name){
    x = X;
    y = Y;
    speed = 0.8;
    w = h = 16;
    life = true;
    state = State;
    //выше инициализация в конструкторе
}

void Bullet::update(float time)
{
    if (life){
        switch (state)
            {
            case up:
                dy = -speed;
                break;
            case down:
                dy = speed;
            }
        y += dy*time;//по у
        if (y <= 0 || y >= SCREEN_H){
            life = false;
        }
        sprite.setPosition(x + w / 2, y + h / 2);//задается позицию пули
    }
}
