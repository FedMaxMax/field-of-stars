#ifndef BONUS_H
#define BONUS_H

#include "entity.h"

class Bonus: public Entity
{
    uint8_t m_valueHeal;    //сколько здоровья прибавиться игроку
public:
    Bonus(Image &p_image, uint16_t p_w, uint16_t p_h, float p_dy, uint8_t p_valueHeal);
    void update();
    uint8_t getValueHeal();
    void setValueHeal(uint8_t p_valueHeal);
};

#endif // BONUS_H
