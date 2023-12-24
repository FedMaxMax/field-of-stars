#ifndef ENTITY_H_
#define ENTITY_H_

#include <SFML/Graphics.hpp>
#include <stdint.h>
#include "global.h"

using namespace sf;

/*
 * Класс Entity описывает графический объект, который может передвигаться по экрану
 * и пересекаться с другими объектами класса Entity и его наследниками.
 */

class Entity {
private:
    Texture m_texture;
    Sprite  m_sprite;
    bool m_alive;      // Показывает является ли объект действующим или его можно удалить
    uint16_t m_collisionDamage; // Количество урона, которое будет нанесено при столкновении с другим объектом

protected:
    float m_x, m_y;    // Координаты распложения верхнего правого угла объекта
    float m_dx, m_dy;  // Скорость движения объекта по осям
    uint16_t m_w, m_h; // Ширина и высота объекта
    int16_t m_dir;     // Направление движения в градусах
    float m_speed;     // Абсолютная скорость

public:
    Entity(float p_x, float p_y, uint16_t p_w, uint16_t p_h);
    virtual void update() = 0; // Эта функция отвечает за изменение скорости и направления движения
                               // Определяет траекторию движения объекта
    void move(float p_time);
    void die();
    void revive();
    void rotate(float p_angle);
    bool isAlive();
    uint16_t getCollisionDamage();
    Sprite * getSprite();
    void setCollisionDamage(uint16_t p_collisionDamage);
    void setImage(Image &p_image);
    FloatRect getRect();
    virtual ~Entity(){}
};

#endif // ENTITY_H
