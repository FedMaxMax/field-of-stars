#ifndef BULLET_H_
#define BULLET_H_

#include <SFML/Graphics.hpp>
#include <stdint.h>
#include "entity.h"

const int BULLET_W = 16;
const int BULLET_H = 16;

/*
 * Класс Bullet описывает снаряды
 */

class Bullet :public Entity{//класс пули
public:
    Bullet(uint16_t p_dir, float p_speed, float p_x, float p_y, uint16_t p_w, uint16_t p_h, uint16_t p_damage);
    void update(); // Эта функция отвечает за изменение скорости и направления движения
                   // Определяет траекторию движения объекта
};

#endif // BULLET_H
