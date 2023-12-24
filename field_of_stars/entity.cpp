#include "entity.h"

Entity::Entity(float p_x, float p_y, uint16_t p_w, uint16_t p_h)
{
    m_x = p_x;
    m_y = p_y;
    m_w = p_w;
    m_h = p_h;

    m_dx = 0;
    m_dy = 0;
    m_speed = 0;
    m_dir = 0;
    m_collisionDamage = 50;
    m_alive = true;
}

void Entity::move(float p_time)
{
    m_x+= m_dx*p_time;
    m_y+= m_dy*p_time;
    m_sprite.setPosition(m_x, m_y);
}

void Entity::die()
{
    m_alive = false;
}

void Entity::revive()
{
    m_alive = true;
}

void Entity::rotate(float p_angle)
{
    m_sprite.rotate(p_angle);
}

bool Entity::isAlive()
{
    return m_alive;
}

uint16_t Entity::getCollisionDamage()
{
    return m_collisionDamage;
}

Sprite *Entity::getSprite()
{
    return &m_sprite;
}

void Entity::setCollisionDamage(uint16_t p_collisionDamage)
{
    m_collisionDamage = p_collisionDamage;
}

void Entity::setImage(Image &p_image)
{
    m_texture.loadFromImage(p_image);
    m_sprite.setTexture(m_texture);
    m_sprite.setTextureRect(IntRect(0, 0, m_w, m_h));
}

FloatRect Entity::getRect(){//метод получения прямоугольника. его коорд, размеры (шир,высот).
    FloatRect rect(m_x, m_y, m_w, m_h); // переменная FR типа FloatRect
    return rect;
    //Тип данных (класс) "sf::FloatRect" позволяет хранить четыре координаты прямоугольника
    //в нашей игре это координаты текущего расположения тайла на карте
    //далее это позволит спросить, есть ли ещё какой-либо тайл на этом месте
    //эта ф-ция нужна для проверки пересечений
}
