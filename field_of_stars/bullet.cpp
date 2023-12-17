#include "bullet.h"
#include <cmath>
#include "global.h"

Bullet::Bullet(Image &p_image, uint16_t p_dir, float p_speed, float p_x, float p_y, uint16_t p_w, uint16_t p_h,  uint16_t p_damage)
    :Entity(p_image, p_x, p_y, p_w, p_h) // Вызываем конструктор базового класса
{
    m_speed = p_speed;
    m_dir = p_dir;
    m_dx = m_speed * cos((float)m_dir/180 * PI);
    m_dy = -m_speed * sin((float)m_dir/180 * PI);
    rotate((float)m_dir/180 * PI);

    setCollisionDamage(p_damage);
}

void Bullet::update()
{
    if (m_x <= 0 || m_x >= SCREEN_W || m_y <= 0 || m_y >= SCREEN_H){
        die();
    }
}
