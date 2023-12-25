#ifndef BATTLESCREEN_H_
#define BATTLESCREEN_H_

#include <SFML/Graphics.hpp>
#include <list>
#include <ctime>
#include "global.h"
#include "playership.h"
#include "entity.h"
#include "enemy.h"
#include "bullet.h"
#include "bonus.h"

using namespace sf;

/*
 * Класс battlecsreen описывает работу игрового экрана и взаимодействия
 * игрока, врагов, снарядов
 */

class BattleScreen
{
private:

    Font createFont();
    void updateObjects(float& p_time, PlayerShip& p_player, std::list<Bullet*>& p_plBullets, std::list<Enemy*>& p_enemies,
                       std::list<Bullet*>& p_enBullets, std::list<Bonus*> &bonuses);
    void collisionCheck(PlayerShip& p_player, std::list<Bullet*>& p_plBullets, std::list<Enemy*>& p_enemies,
                        std::list<Bullet*>& p_enBullets, std::list<Bonus*> &bonuses);
    void draw(RenderWindow& p_window,PlayerShip& p_player, std::list<Bullet*>& p_plBullets, std::list<Enemy*>& p_enemies,
              std::list<Bullet*>& p_enBullets, std::list<Bonus*> &bonuses);
    void respawnEnemy(float& p_time, std::list<Enemy*>& p_enemies, Image p_enemyCommonImage, Image p_enemyThreeBulletImage, Image p_enemyStrongImage,
                      Image p_bulletCommonImage, Image p_bulletThreeBulletImage, Image p_bulletStrongImage);
    void spawnBonus(float p_time, std::list<Bonus*>& p_bonuses, Image &p_bonusImage);
public:
    void play();

};

#endif // BATTLESCREEN_H
