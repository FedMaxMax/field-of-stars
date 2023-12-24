#include "battlescreen.h"
#include <SFML/Graphics.hpp>
#include <list>
#include <ctime>
#include "global.h"
#include "playership.h"
#include "entity.h"
#include "enemy.h"
#include "bullet.h"
#include "battlescreen.h"
#include "textwindow.h"
/*
 *VideoMode desktop = sf::VideoMode::getDesktopMode();
 */
BattleScreen::BattleScreen(): m_window {sf::VideoMode(SCREEN_W, SCREEN_H, sf::VideoMode::getDesktopMode().bitsPerPixel), "Field of stars"},
                              m_player{(float) ((SCREEN_W - 96)/2), (float) (SCREEN_H - 10 - 96), 96, 96}//объект класса игрока
{
    m_map_image.loadFromFile("images/map.png");//загружаем файл для карты
    m_heroImage.loadFromFile("images/Hero_Sprite.png"); // загружаем изображение игрока
    m_enemyImage[0].loadFromFile("images/Enemy_1_3.png"); // загружаем изображение врага
    m_bulletImage[0].loadFromFile("images/Hero_bullet.png"); //изображение для пули

    m_player.setImage(m_heroImage);
    m_player.setBulletImage(m_bulletImage[0]);

}

Font BattleScreen::createFont()
{
    Font font;//шрифт
    font.loadFromFile("CyrilicOld.ttf");//передаем нашему шрифту файл шрифта
    Text text("", font, 20);//создаем объект текст
    text.setColor(Color::Red);//покрасили текст в красный
    text.setStyle(Text::Bold);//жирный текст.
    return font;
}


void BattleScreen::updateObjects(float& p_time)
{
    std::list<Bullet*>::iterator itBul;
    std::list<Enemy*>::iterator itEn;
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
}

void BattleScreen::collisionCheck()
{
    std::list<Bullet*>::iterator itBul;
    std::list<Enemy*>::iterator itEn;
    for(itBul = m_enBullets.begin(); itBul != m_enBullets.end(); itBul++) // Проверяем столкновение пуль с игроком
    {
        if(m_player.getRect().intersects((*itBul)->getRect()))
        {
            m_player.getDamaged((*itBul)->getCollisionDamage());
            (*itBul)->die();
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
}

void BattleScreen::respawnEnemy(float& p_time)
{
    uint16_t enType;
    static float respawnTimer = 0;
    if (m_enemies.size() < ENEMY_COUNT)
    {
        respawnTimer+= p_time;
        if(respawnTimer > 1000)
        {
            enType = rand() % 10 + 1;
            if (enType <= 5)
                {
                m_enemies.push_back(new Enemy(96, 96, "common"));
                m_enemies.back()->setImage(m_enemyImage[0]);
                m_enemies.back()->setBulletImage(m_bulletImage[0]);
                respawnTimer = 0;
                }

            else if ((enType > 5) && (enType < 9))
                {
                m_enemies.push_back(new Enemy(96, 96, "threebullet"));
                m_enemies.back()->setImage(m_enemyImage[0]);
                m_enemies.back()->setBulletImage(m_bulletImage[0]);
                respawnTimer = 0;
                }

            else if (enType >= 9)
                {
                m_enemies.push_back(new Enemy(96, 96, "strong"));
                m_enemies.back()->setImage(m_enemyImage[0]);
                m_enemies.back()->setBulletImage(m_bulletImage[0]);
                respawnTimer = 0;
                }

        }
    }
}


void BattleScreen::play()
{

    srand(time(0));
    Font font = createFont();//шрифт

    Texture map;//текстура карты
    map.loadFromImage(m_map_image);//заряжаем текстуру картинкой
    Sprite s_map;//создаём спрайт для карты
    s_map.setTexture(map);//заливаем текстуру спрайтом

    Clock clock;

    for (int i = 0; i < 5;i++)
        {
        m_enemies.push_back(new Enemy(96, 96, "common"));
        m_enemies.back()->setImage(m_enemyImage[0]);
        m_enemies.back()->setBulletImage(m_bulletImage[0]);
        }


    while (m_window.isOpen())

    {
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time = time/1000;


        if (!m_player.isAlive())
        {
            m_window.clear();

            TextWindow loseWindow{(SCREEN_W/4 + 40), (SCREEN_H/4) , "      You lose\n\n R         -  Repeat\n Esc     -  Quit Game",
                        50, Color::Red, font, (SCREEN_H / 2), (SCREEN_W / 2), Color::Black};
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
                    m_window.clear();
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
                    for (int i = 0; i < 5;i++)
                    {
                        m_enemies.push_back(new Enemy(96, 96, "common"));
                        m_enemies.back()->setImage(m_enemyImage[0]);
                        m_enemies.back()->setBulletImage(m_bulletImage[0]);
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

            respawnEnemy(time);

            updateObjects(time);

            collisionCheck();

            m_window.clear(); // Внимание! Порядок отрисовки важен! Порядок отрисовки: Карта - Пули - Враги - Игрок
            m_window.draw(s_map); // Рисуем фон

            draw();

            m_window.display();        }
    }
}
