#include "bonus.h"

Bonus::Bonus(uint16_t p_w, uint16_t p_h, float p_dy, uint16_t p_recovery): Entity(rand()%(SCREEN_W - p_w*3) + p_w, -p_h, p_w, p_h)
{
    m_recovery = p_recovery;
    m_dy = p_dy;
}

void Bonus::update()
{
    if (m_y > SCREEN_H){
        die();
    }
}

uint16_t Bonus::getRecovery()
{
    return m_recovery;
}

void Bonus::setRecovery(uint16_t p_recovery)
{
    m_recovery = p_recovery;
}
