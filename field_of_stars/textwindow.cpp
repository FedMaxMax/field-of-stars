#include "textwindow.h"
#include <iostream>
#include <string>

TextWindow::TextWindow(uint32_t p_x,
                       uint32_t p_y,
                       std::string p_title,
                       uint8_t p_size,
                       sf::Color p_color,
                       uint32_t p_width,
                       uint32_t p_height,
                       sf::Color p_colorBackground)
                        : m_background{sf::Vector2f(p_width,p_height)}
{
    m_background.setFillColor(p_colorBackground);
    m_background.setPosition(p_x,p_y);
    m_title.setCharacterSize(p_size);
    m_title.setString(p_title);
    m_title.setPosition(p_x + 10,p_y);
    m_title.setColor(p_color);
}

void TextWindow::editTitle(std::string p_title)
{
    m_title.setString(p_title);
}

void TextWindow::draw(sf::RenderWindow& p_window)
{
    p_window.draw(m_background);
    p_window.draw(m_title);
}

void TextWindow::setFont(sf::Font &p_font)
{
    m_title.setFont(p_font);
}
