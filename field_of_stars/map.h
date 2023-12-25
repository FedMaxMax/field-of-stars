#ifndef MAP_H_
#define MAP_H_

#include <SFML/Graphics.hpp>
#include <stdint.h>

using namespace sf;

const int MAP_W = 800;
const int MAP_H = 2400;

class Map
{

private:
    Texture m_texture;
    Sprite m_sprite;
    Sprite m_extraSprite;
    float m_position;

public:
    Map();
    void setImage(Image& p_image);
    void update(float p_time);
    void draw(RenderWindow& p_window);
    void resetPosition();
};

#endif // MAP_H_
