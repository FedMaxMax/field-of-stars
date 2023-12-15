#ifndef BULLET_H
#define BULLET_H

#include "global.h"
#include "entity.h"

////////////////////////////КЛАСС ПУЛИ////////////////////////
class Bullet :public Entity{//класс пули
public:
    Bullet(Image &image, float X, float Y, int W, int H, std::string Name, StateObject State);
    void update(float time);
};

#endif // BULLET_H
