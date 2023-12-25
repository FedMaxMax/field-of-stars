#ifndef BATTLESCREEN_H_
#define BATTLESCREEN_H_

#include <SFML/Graphics.hpp>
#include <list>
#include <ctime>
#include <array>
#include "global.h"
#include "playership.h"
#include "playerstate.h"
#include "entity.h"
#include "enemy.h"
#include "bullet.h"
#include "map.h"

using namespace sf;

/*
 * Класс battlecsreen описывает работу игрового экрана и взаимодействия
 * игрока, врагов, снарядов
 */

class BattleScreen
{
private:
    uint32_t m_playerScore; // счет игрока

    RenderWindow m_window;
    Image m_map_image;
    Image m_heroImage;
    std::array<Image, 3> m_enemyImage; // Массив изображений врагов
    std::array<Image, 4> m_bulletImage; // Массив изображений пуль

    Map m_map;
    PlayerShip m_player;
    PlayerState m_state;
    std::list<Bullet*> m_enBullets; // Список вражеских пуль
    std::list<Bullet*> m_plBullets; // Список пуль игрока
    std::list<Enemy*> m_enemies; // Список пуль

    void updateObjects(float p_time);
    void collisionCheck();
    void draw();
    void respawnEnemy(float p_time);

public:
    BattleScreen();
    void play();

};

#endif // BATTLESCREEN_H
