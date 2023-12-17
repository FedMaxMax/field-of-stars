#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include <string>
#include "unit.h"
#include "bullet.h"

using namespace sf;

/*
 * Класс Enemy описывает юнит, который передвигается по экрану
 * и стреляет без участия игрока
 */

class Enemy :public Unit{
private:
    std::string m_type;
    uint16_t m_cost;

protected:
    virtual void shoot(std::list<Bullet*>& p_bulletList); // Используется лист, так как пуль может быть несколько

public:
    Enemy(Image &p_image, Image &p_bulletImage, uint16_t p_w, uint16_t p_h, std::string p_type);
    void update(); // Эта функция отвечает за изменение скорости и направления движения
                   // Определяет траекторию движения объекта
    uint16_t getCost();
};
#endif // ENEMY_H
