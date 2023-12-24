#include "playerstate.h"
#include <string>
#include "global.h"

PlayerState::PlayerState(sf::Font &p_font, uint16_t p_score, uint16_t p_health)
    : TextWindow{10, 10, "Health: " + std::to_string(p_health) + "\nScore: 0", 26, sf::Color::Red,
                 p_font, 180, 75, DARKBLUE}
{

    m_score = p_score;
    m_health = p_health;
}

void PlayerState::editTitle(std::string p_title)
{
    TextWindow::editTitle(p_title);
}

void PlayerState::setScore(uint16_t p_score)
{
    m_score = p_score;
    std::string newTitle = {"Health: " + std::to_string(m_health) + "\nScore: " + std::to_string(m_score)};
    editTitle(newTitle);
}

void PlayerState::setHealth(uint16_t p_health)
{
    m_health = p_health;
    std::string newTitle = {"Health: " + std::to_string(m_health) + "\nScore: " + std::to_string(m_score)};
    editTitle(newTitle);
}
