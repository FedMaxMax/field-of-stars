#include "unit.h"

Unit::Unit(Image &p_image, Image &p_bulletImage, float p_x, float p_y, uint16_t p_w, uint16_t p_h, uint16_t p_health, uint16_t p_shootTime)
    : Entity(p_image, p_x, p_y, p_w, p_h) // Вызываем конструктор базового класса и инициализируем константу
{
    m_bulletImage = p_bulletImage;
    m_shootTime = p_shootTime;
    m_health = p_health;
}

void Unit::increaseShootTimer(float p_time, std::list<Bullet*>& p_bulletList)
{
    if(isAlive())
    {
        m_shootTimer += p_time;

        if(m_shootTimer >= m_shootTime)
        {
            shoot(p_bulletList);
            m_shootTimer = 0;
        }
    }
}

void Unit::setBulletImage(Image &p_bulletImage)
{
    m_bulletImage = p_bulletImage;
}

void Unit::setHealth(uint16_t p_health)
{
    m_health = p_health;
}

void Unit::addHealth(uint16_t p_health)
{
    m_health += p_health;
}

uint16_t Unit::getHealth()
{
    return m_health;
}

bool Unit::getDamaged(uint16_t p_damage)
{
    if(isAlive())
    {
        if(m_health > p_damage)
        {
            m_health -= p_damage;
        }
        else
        {
            m_health = 0;
            die();
        }
    }
    return isAlive();
}
