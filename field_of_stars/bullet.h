#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>
#include <stdint.h>
#include "entity.h"

/*
 * Класс Bullet описывает снаряды
 */

class Bullet :public Entity{//класс пули
public:
    Bullet(Image &p_image, uint16_t p_dir, float p_speed, float p_x, float p_y, uint16_t p_w, uint16_t p_h, uint16_t p_damage);
    void update(); // Эта функция отвечает за изменение скорости и направления движения
                   // Определяет траекторию движения объекта
};

#endif // BULLET_H
