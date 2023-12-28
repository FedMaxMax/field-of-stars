#include "battlescreen.h"
#include <SFML/Graphics.hpp>
#include <list>
#include <ctime>
#include <string>
#include "global.h"
#include "playership.h"
#include "entity.h"
#include "enemy.h"
#include "bullet.h"
#include "playerstate.h"
#include "battlescreen.h"
#include "textwindow.h"
#include "map.h"
#include "bonus.h"

BattleScreen::BattleScreen(): m_window {sf::VideoMode(SCREEN_W, SCREEN_H, sf::VideoMode::getDesktopMode().bitsPerPixel), "Field of stars"},
                              m_player{(float) ((SCREEN_W - PLAYER_W)/2), (float) (SCREEN_H - 10 - PLAYER_H), PLAYER_W, PLAYER_H},//объект класса игрока
                              m_state{0, 100} // Меню с информацией о здоровье и очках игрока
{
    m_playerScore = 0;
    m_bonusTimer = 0;
    m_map_image.loadFromFile("images/map.png");//загружаем файл для карты
    m_heroImage.loadFromFile("images/Hero_Sprite.png"); // загружаем изображение игрока
    m_bonusImage.loadFromFile("images/health_point.png");
    m_enemyImage[0].loadFromFile("images/Enemy_1_3.png"); // загружаем изображение врага
    m_enemyImage[1].loadFromFile("images/Enemy_2.png");
    m_enemyImage[2].loadFromFile("images/Enemy_3.png");
    m_bulletImage[0].loadFromFile("images/Hero_bullet.png"); //изображение для пули
    m_bulletImage[1].loadFromFile("images/Enemy_bullet_1.png");
    m_bulletImage[2].loadFromFile("images/Enemy_bullet_2.png");
    m_bulletImage[3].loadFromFile("images/Enemy_bullet_3.png");
    m_enemyCount = 4;
    m_difficultyTimer = 0;
    m_difficultyTrigger = 20000;    //20 секунд

    m_player.setImage(m_heroImage);
    m_player.setBulletImage(m_bulletImage[0]);
    m_map.setImage(m_map_image);
}

void BattleScreen::updateObjects(float p_time)
{
    std::list<Bullet*>::iterator itBul;
    std::list<Enemy*>::iterator itEn;
    std::list<Bonus*>::iterator itBon;

    m_map.update(p_time);

    for (itBul = m_enBullets.begin(); itBul != m_enBullets.end(); itBul++) // Обновляем пули. Должно быть перед обновлением врагов
                                                                   // и игрока, так как они могут создать новые!
    {
        (*itBul)->update();
        (*itBul)->move(p_time);
    }

    for (itBul = m_plBullets.begin(); itBul != m_plBullets.end(); itBul++)
    {
        (*itBul)->update();
        (*itBul)->move(p_time);
    }

    m_player.update(); // Обновляем игрока
    m_player.move(p_time);
    m_player.increaseShootTimer(p_time, m_plBullets);

    for(itEn = m_enemies.begin(); itEn != m_enemies.end(); itEn++) // Обновляем врагов
    {
        (*itEn)->update();
        (*itEn)->move(p_time);
        (*itEn)->increaseShootTimer(p_time, m_enBullets);
    }

    for(itBon = m_bonuses.begin(); itBon != m_bonuses.end(); itBon++) // Обновляем бонусы
    {
        (*itBon)->update();
        (*itBon)->move(p_time);
    }
}

void BattleScreen::collisionCheck()
{
    std::list<Bullet*>::iterator itBul;
    std::list<Enemy*>::iterator itEn;
    std::list<Bonus*>::iterator itBon;

    for(itBul = m_enBullets.begin(); itBul != m_enBullets.end(); itBul++) // Проверяем столкновение пуль с игроком
    {
        if(m_player.getRect().intersects((*itBul)->getRect()))
        {
            m_player.getDamaged((*itBul)->getCollisionDamage());
            m_state.setHealth(m_player.getHealth());
            (*itBul)->die();
        }
    }
    for(itBon = m_bonuses.begin(); itBon != m_bonuses.end(); ++itBon) // Проверяем столкновение бонусов с игроком
    {
        if (m_player.getRect().intersects((*itBon)->getRect()))
        {
            m_player.addHealth((*itBon)->getRecovery());
            m_state.setHealth(m_player.getHealth());
            (*itBon)->die();
        }
    }

    for(itBul = m_plBullets.begin(); itBul != m_plBullets.end(); itBul++) // Проверяем столкновение пуль с врагами
    {
        if((*itBul)->isAlive())
        {
            for(itEn = m_enemies.begin(); itEn != m_enemies.end() && (*itBul)->isAlive(); itEn++)
            {
                if((*itEn)->isAlive())
                {
                    if((*itEn)->getRect().intersects((*itBul)->getRect()))
                    {
                        (*itEn)->getDamaged((*itBul)->getCollisionDamage());
                        (*itBul)->die();

                        if(!(*itEn)->isAlive())
                        {
                            m_playerScore += (*itEn)->getCost();
                            m_state.setScore(m_playerScore);
                        }
                    }
                }
            }
        }
    }
}

void BattleScreen::draw()
{
    std::list<Bullet*>::iterator itBul;
    std::list<Enemy*>::iterator itEn;
    std::list<Bonus*>::iterator itBon;

    m_map.draw(m_window);

    for(itBon = m_bonuses.begin(); itBon != m_bonuses.end(); itBon++) // Обновляем бонусы
    {
        if ((*itBon)->isAlive())
        {
            m_window.draw(*((*itBon)->getSprite()));
        }
        else
        {
            delete *itBon;
            m_bonuses.erase(itBon);
        }
    }

    for(itBul = m_enBullets.begin(); itBul != m_enBullets.end(); itBul++) // Рисуем вражеские пули (удаляем если уничтожены)
    {
        if((*itBul)->isAlive())
        {
            m_window.draw(*((*itBul)->getSprite()));
        } else {
            delete *itBul;
            m_enBullets.erase(itBul);
        }
    }

    for(itBul = m_plBullets.begin(); itBul != m_plBullets.end(); itBul++) // Рисуем пули игрока (удаляем если уничтожены)
    {
        if((*itBul)->isAlive())
        {
            m_window.draw(*((*itBul)->getSprite()));
        } else {
            delete *itBul;
            m_plBullets.erase(itBul);
        }

    }

    for(itEn = m_enemies.begin(); itEn != m_enemies.end(); itEn++) // Рисуем врагов (удаляем если уничтожены)
    {
        if((*itEn)->isAlive())
        {
            m_window.draw(*((*itEn)->getSprite()));
        } else {
            delete *itEn;
            m_enemies.erase(itEn);
        }
    }

    m_window.draw(*(m_player.getSprite())); // Рисуем игрока
    m_state.draw(m_window);
}

void BattleScreen::respawnEnemy(float p_time)
{
    uint16_t enType;
    static float respawnTimer = 0;
    if (m_enemies.size() < m_enemyCount)
    {
        respawnTimer+= p_time;
        if(respawnTimer > 750)
        {
            enType = rand() % 10 + 1;
            if (enType <= 6)
                {
                m_enemies.push_back(new Enemy(ENEMY_W, ENEMY_H, "common"));
                m_enemies.back()->setImage(m_enemyImage[0]);
                m_enemies.back()->setBulletImage(m_bulletImage[1]);
                respawnTimer = 0;
                }

            else if ((enType > 6) && (enType < 9))
                {
                m_enemies.push_back(new Enemy(ENEMY_W, ENEMY_H, "threebullet"));
                m_enemies.back()->setImage(m_enemyImage[1]);
                m_enemies.back()->setBulletImage(m_bulletImage[2]);
                respawnTimer = 0;
                }

            else if (enType >= 9)
                {
                m_enemies.push_back(new Enemy(ENEMY_W, ENEMY_H, "strong"));
                m_enemies.back()->setImage(m_enemyImage[2]);
                m_enemies.back()->setBulletImage(m_bulletImage[3]);
                respawnTimer = 0;
                }

        }
    }
}

void BattleScreen::spawnBonus(float p_time)
{
    m_bonusTimer+= p_time;
    if (m_bonusTimer > 12000)  //бонусы падают примерно раз в 12 секунд
    {
        m_bonusTimer = 0;
        m_bonuses.push_back(new Bonus(BONUS_W, BONUS_H, 30));
        m_bonuses.back()->setImage(m_bonusImage);
    }
}

void BattleScreen::updateDifficulty(float p_time)
{
    m_difficultyTimer += p_time;
    if (m_difficultyTimer > m_difficultyTrigger)
        {
        ++m_enemyCount;
        m_difficultyTrigger += 20000;
        m_difficultyTimer = 0;
        }
}

void BattleScreen::play()
{
    srand(time(0));

    Font font;//шрифт
    font.loadFromFile("Visitor Rus.ttf");
    m_state.setFont(font);

    Clock clock;

    for (int i = 0; i < m_enemyCount;i++)
    {
        m_enemies.push_back(new Enemy(ENEMY_W, ENEMY_H, "common"));
        m_enemies.back()->setImage(m_enemyImage[0]);
        m_enemies.back()->setBulletImage(m_bulletImage[1]);
    }


    while (m_window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time = time/1000;


        if (!m_player.isAlive())
        {
            m_window.clear();

            TextWindow loseWindow{SCREEN_W/4-25, SCREEN_H/4, "         You lose\n Your score: " + std::to_string(m_playerScore)
                        + "\n R         -  Repeat\n Esc     -  Quit Game", 50, Color::Red, SCREEN_W/2+85, SCREEN_H/2 - 60, DARKBLUE};
            loseWindow.setFont(font);
            loseWindow.draw(m_window);
            m_window.display();
            clock.restart();
            sf::Event choice;
            while (m_window.pollEvent(choice))
            {
                if (Keyboard::isKeyPressed(Keyboard::R))
                {
                    m_player.revive();
                    m_player.setHealth(100);
                    m_playerScore = 0;
                    m_bonusTimer = 0;
                    m_state.setHealth(100);
                    m_state.setScore(m_playerScore);
                    m_enemyCount = 4;
                    m_difficultyTimer = 0;
                    m_difficultyTrigger = 20000;
                    m_map.resetPosition();
                    m_window.clear();
                    while (!m_bonuses.empty())
                    {
                        delete m_bonuses.back();
                        m_bonuses.pop_back();
                    }
                    while (!m_enemies.empty())
                    {
                        delete m_enemies.back();
                        m_enemies.pop_back();
                    }
                    while(!m_enBullets.empty())
                    {
                        delete m_enBullets.back();
                        m_enBullets.pop_back();
                    }
                    while(!m_plBullets.empty())
                    {
                        delete m_plBullets.back();
                        m_plBullets.pop_back();

                    }
                    for (int i = 0; i < m_enemyCount;i++)
                    {
                        m_enemies.push_back(new Enemy(ENEMY_W, ENEMY_H, "common"));
                        m_enemies.back()->setImage(m_enemyImage[0]);
                        m_enemies.back()->setBulletImage(m_bulletImage[1]);
                    }
                }

                else if (Keyboard::isKeyPressed(Keyboard::Escape))
                {
                    choice.type = sf::Event::Closed;
                }
                if (choice.type == sf::Event::Closed)
                    m_window.close();
            }
         }

        else
        {
            sf::Event event;
            while (m_window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    m_window.close();

                while (Keyboard::isKeyPressed(Keyboard::P)) // Останавливаем если нажата клавиша P
                    clock.restart();
            }

            updateDifficulty(time);
            respawnEnemy(time);
            spawnBonus(time);
            updateObjects(time);
            collisionCheck();

            m_window.clear();

            draw();

            m_window.display();
        }
    }
}
