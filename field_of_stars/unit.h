#ifndef UNIT_H_
#define UNIT_H_

#include <SFML/Graphics.hpp>
#include <stdint.h>
#include <list>
#include "entity.h"
#include "bullet.h"

using namespace sf;

/*
 * Класс Unit описывает объект, который обладает здоровьем и
 * может стрелять с заданым минимальным временем между выстрелами
 */

class Unit : public Entity
{
private:
    uint16_t m_health;  // Здоровье объекта
    uint16_t m_maxHealth;

protected:
    Image m_bulletImage;
    uint16_t m_shootTime; // Минимальное время между выстрелами
    float m_shootTimer; // Таймер, по которому происходит выстрел

    virtual void shoot(std::list<Bullet*>& p_bulletList) = 0; // Используется лист, так как пуль может быть несколько

public:
    Unit(Image &p_image, Image &p_bulletImage, float p_x, float p_y, uint16_t p_w, uint16_t p_h, uint16_t p_health, uint16_t p_shootTime);
    void increaseShootTimer(float p_time, std::list<Bullet*>& p_bulletList); // Добавляем время в таймер выстрела.
                                                    // Если выстрел произошел метод добавляет пули в лист
                                                    // Используется лист, потому что пуль может быть больше чем одна.
    void setBulletImage(Image &p_bulletImage);
    void setHealth(uint16_t p_health);
    void addHealth(uint16_t p_health);
    uint16_t getHealth();
    bool getDamaged(uint16_t p_damage); // Получение урона
};

#endif // UNIT_H
