#include "map.h"
#include "global.h"


Map::Map()
{
    m_position = (MAP_H - SCREEN_H);

}

void Map::setImage(Image &p_image)
{
    m_texture.loadFromImage(p_image);
    m_sprite.setTexture(m_texture);
    m_extraSprite.setTexture(m_texture);
    m_sprite.setTextureRect(IntRect(0, m_position, SCREEN_W, SCREEN_H));
    m_extraSprite.setTextureRect(IntRect(0, MAP_H + m_position, SCREEN_W, SCREEN_H));
    m_sprite.setPosition(0, 0);
}

void Map::update(float p_time)
{
    m_position -= p_time * 0.08;
    if (m_position > 0)
    {
        m_sprite.setTextureRect(IntRect(0, m_position, SCREEN_W, SCREEN_H));
    }
    else if(m_position <= 0)
    {
        m_extraSprite.setTextureRect(IntRect(0, MAP_H + m_position - 1, SCREEN_W, -m_position + 1));
        m_sprite.setTextureRect(IntRect(0, 0, SCREEN_W, SCREEN_H));
        m_sprite.setPosition(0, -m_position);
    }
    if (m_position < (-SCREEN_H))
    {
        m_position = (MAP_H - SCREEN_H);
        m_sprite.setTextureRect(IntRect(0, m_position, SCREEN_W, SCREEN_H));
        m_sprite.setPosition(0, 0);
    }
}


void Map::draw(RenderWindow &p_window)
{
    p_window.draw(m_sprite);
    if(m_position < 0)
    {
        p_window.draw(m_extraSprite);
    }
}

void Map::resetPosition()
{
    m_position = (MAP_H - SCREEN_H);
    m_sprite.setTextureRect(IntRect(0, m_position, SCREEN_W, SCREEN_H));
    m_extraSprite.setTextureRect(IntRect(0, MAP_H + m_position, SCREEN_W, SCREEN_H));
    m_sprite.setPosition(0, 0);
}

