#include "bonus.h"

Bonus::Bonus(Image &p_image, uint16_t p_w, uint16_t p_h, float p_dy, uint8_t p_valueHeal):Entity(p_image, rand()%(SCREEN_W - p_w*2) + p_w, 0, p_w, p_h)
{
m_valueHeal = p_valueHeal;
m_dy = p_dy;
}

void Bonus::update()
{
    if (m_y > SCREEN_H){
        die();
    }
}

uint8_t Bonus::getValueHeal()
{
    return m_valueHeal;
}

void Bonus::setValueHeal(uint8_t p_valueHeal)
{
    m_valueHeal = p_valueHeal;
}
