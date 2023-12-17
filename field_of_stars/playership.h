#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <stdint.h>
#include <list>
#include "bullet.h"
#include "unit.h"

using namespace sf;

/*
 * Класс PlayerShip описывает юнит, который находится под управлением игрока
 */

class PlayerShip :public Unit
{
private:
    uint16_t m_playerScore; // счет игрока

    void control(); // Функция, где происходит считывание нажатий клавиш игроком
    virtual void shoot(std::list<Bullet *>& p_bulletList); // Используется лист, так как пуль может быть несколько
public:
    PlayerShip(Image &p_image, Image &p_bulletImage, float p_x, float p_y, uint16_t p_w, uint16_t p_h);
    void update(); // Эта функция отвечает за изменение скорости и направления движения
                   // Определяет траекторию движения объекта
    void increaseShootTimer(float p_time, std::list<Bullet*>& p_bulletList); // Добавляем время в таймер выстрела.
                                                    // Если выстрел произошел метод добавляет пули в лист
                                                    // Используется лист, потому что пуль может быть больше чем одна.
    void increaseScore(uint16_t p_award);
    void die();
    bool getDamaged(uint16_t p_damage);
};

#endif // PLAYER_H
