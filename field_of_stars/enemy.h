#ifndef ENEMY_H
#define ENEMY_H

#include "entity.h"
#include "bullet.h"

////////////////////////////КЛАСС ВРАГА////////////////////////
class Enemy :public Entity{
public:
    int enemiesBulletsTimer;
    int direction;//направление движения врага
    Enemy(Image &image, float X, float Y, int W, int H, std::string Name);
    void checkCollisionWithBounds();//ф-ция проверки столкновений с картой
    void update(float time);
    Bullet* strike(Image& BulletImage);
};//класс Enemy закрыт
#endif // ENEMY_H
