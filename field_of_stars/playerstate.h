#ifndef PLAYERSTATE_H
#define PLAYERSTATE_H

#include "textwindow.h"
#include <stdint.h>
#include <SFML/Graphics.hpp>

class PlayerState : public TextWindow
{
private:
    uint16_t m_score;
    uint16_t m_health;

    void editTitle(std::string p_title);
public:
    PlayerState(sf::Font &p_font, uint16_t p_score, uint16_t p_health);
    void setScore(uint16_t p_score);
    void setHealth(uint16_t p_health);
};

#endif // PLAYERSTATE_H
