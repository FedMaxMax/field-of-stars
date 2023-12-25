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
#include "bonus.h"

Font BattleScreen::createFont()
{
    Font font;//шрифт
    font.loadFromFile("CyrilicOld.ttf");//передаем нашему шрифту файл шрифта
    Text text("", font, 20);//создаем объект текст
    text.setColor(Color::Red);//покрасили текст в красный
    text.setStyle(Text::Bold);//жирный текст.
    return font;
}


void BattleScreen::updateObjects(float& p_time, PlayerShip& p_player, std::list<Bullet*>& p_plBullets, std::list<Enemy*>& p_enemies,
                                 std::list<Bullet*>& p_enBullets, std::list<Bonus*> &bonuses)
{
    std::list<Bullet*>::iterator itBul;
    std::list<Enemy*>::iterator itEn;

    for (itBul = p_enBullets.begin(); itBul != p_enBullets.end(); itBul++) // Обновляем пули. Должно быть перед обновлением врагов
                                                                   // и игрока, так как они могут создать новые!
    {
        (*itBul)->update();
        (*itBul)->move(p_time);
    }

    for (itBul = p_plBullets.begin(); itBul != p_plBullets.end(); itBul++)
    {
        (*itBul)->update();
        (*itBul)->move(p_time);
    }

    p_player.update(); // Обновляем игрока
    p_player.move(p_time);
    p_player.increaseShootTimer(p_time, p_plBullets);

    for(itEn = p_enemies.begin(); itEn != p_enemies.end(); itEn++) // Обновляем врагов
    {
        (*itEn)->update();
        (*itEn)->move(p_time);
        (*itEn)->increaseShootTimer(p_time, p_enBullets);
    }

    for(std::list<Bonus*>::iterator it = bonuses.begin(), end = bonuses.end(); it != end; ++it) // Обновляем бонусы
    {
        (*it)->update();
        (*it)->move(p_time);
    }
}

void BattleScreen::collisionCheck(PlayerShip& p_player, std::list<Bullet*>& p_plBullets, std::list<Enemy*>& p_enemies,
                                  std::list<Bullet*>& p_enBullets, std::list<Bonus*> &bonuses)
{
    std::list<Bullet*>::iterator itBul;
    std::list<Enemy*>::iterator itEn;
    for(itBul = p_enBullets.begin(); itBul != p_enBullets.end(); itBul++) // Проверяем столкновение пуль с игроком
    {
        if(p_player.getRect().intersects((*itBul)->getRect()))
        {
            p_player.getDamaged((*itBul)->getCollisionDamage());
            (*itBul)->die();
        }
    }
    for(std::list<Bonus*>::iterator it = bonuses.begin(), end = bonuses.end(); it != end; ++it) // Проверяем столкновение бонусов с игроком
    {
        if (p_player.getRect().intersects((*it)->getRect()))
            {
            p_player.addHealth((*it)->getValueHeal());
            (*it)->die();
            }
    }

    for(itBul = p_plBullets.begin(); itBul != p_plBullets.end(); itBul++) // Проверяем столкновение пуль с врагами
    {
        if((*itBul)->isAlive())
        {
            for(itEn = p_enemies.begin(); itEn != p_enemies.end() && (*itBul)->isAlive(); itEn++)
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

void BattleScreen::draw(RenderWindow& p_window,PlayerShip& p_player, std::list<Bullet*>& p_plBullets, std::list<Enemy*>& p_enemies,
                        std::list<Bullet*>& p_enBullets, std::list<Bonus*> &bonuses)
{
    std::list<Bullet*>::iterator itBul;
    std::list<Enemy*>::iterator itEn;

    for(std::list<Bonus*>::iterator it = bonuses.begin(), end = bonuses.end(); it != end; ) // Обновляем бонусы
    {
        if ((*it)->isAlive())
            {
            p_window.draw(*((*it)->getSprite()));
            ++it;
            }
        else
            {
            delete *it;
            it = bonuses.erase(it);
            }
    }

    for(itBul = p_enBullets.begin(); itBul != p_enBullets.end(); itBul++) // Рисуем вражеские пули (удаляем если уничтожены)
    {
        if((*itBul)->isAlive())
        {
            p_window.draw(*((*itBul)->getSprite()));
        } else {
            delete *itBul;
            p_enBullets.erase(itBul);
        }
    }

    for(itBul = p_plBullets.begin(); itBul != p_plBullets.end(); itBul++) // Рисуем пули игрока (удаляем если уничтожены)
    {
        if((*itBul)->isAlive())
        {
            p_window.draw(*((*itBul)->getSprite()));
        } else {
            delete *itBul;
            p_plBullets.erase(itBul);
        }

    }

    for(itEn = p_enemies.begin(); itEn != p_enemies.end(); itEn++) // Рисуем врагов (удаляем если уничтожены)
    {
        if((*itEn)->isAlive())
        {
            p_window.draw(*((*itEn)->getSprite()));
        } else {
            delete *itEn;
            p_enemies.erase(itEn);
        }
    }

    p_window.draw(*(p_player.getSprite())); // Рисуем игрока
}

void BattleScreen::respawnEnemy(float& p_time, std::list<Enemy*>& p_enemies, Image p_enemyCommonImage, Image p_enemyThreeBulletImage, Image p_enemyStrongImage,
                                Image p_bulletCommonImage, Image p_bulletThreeBulletImage, Image p_bulletStrongImage)
{
    uint16_t enType;
    static float respawnTimer = 0;
    if (p_enemies.size() < ENEMY_COUNT)
    {
        respawnTimer+= p_time;
        if(respawnTimer > 1000)
        {
            enType = rand() % 10 + 1;
            if (enType <= 5)
                {
                p_enemies.push_back(new Enemy(p_enemyCommonImage, p_bulletCommonImage, ENEMY_W, ENEMY_H, "common"));
                respawnTimer = 0;
                }

            else if ((enType > 5) && (enType < 9))
                {
                p_enemies.push_back(new Enemy(p_enemyThreeBulletImage, p_bulletStrongImage, ENEMY_W, ENEMY_H, "strong"));
                respawnTimer = 0;
                }

            else if (enType >= 9)
                {
                p_enemies.push_back(new Enemy(p_enemyStrongImage, p_bulletThreeBulletImage, ENEMY_W, ENEMY_H, "threebullet"));
                respawnTimer = 0;
                }

        }
    }
}

void BattleScreen::spawnBonus(float p_time, std::list<Bonus*> &p_bonuses, Image &p_bonusImage)
{
    static double spawnTimer = 0;
    spawnTimer+= p_time;
    if (spawnTimer > 15000)    //бонусы падают примерно раз в 15 секунд
        {
        spawnTimer = 0;
        p_bonuses.push_back(new Bonus(p_bonusImage, 19, 17, 0.1, 50));
        }

}

void BattleScreen::play()
{

    srand(time(0));
    VideoMode desktop = sf::VideoMode::getDesktopMode();
    RenderWindow window(sf::VideoMode(SCREEN_W, SCREEN_H, desktop.bitsPerPixel), "Field of stars");

    Font font = createFont();//шрифт

    Image map_image;//объект изображения для карты
    map_image.loadFromFile("images/map.png");//загружаем файл для карты
    Image heroImage;
    heroImage.loadFromFile("images/Hero_Sprite.png"); // загружаем изображение игрока

    Image enemyCommonImage;
    enemyCommonImage.loadFromFile("images/Enemy_1_3.png"); // загружаем изображение врага
    Image enemyThreeBulletImage;
    enemyThreeBulletImage.loadFromFile("images/Enemy_3.png"); // загружаем изображение врага
    Image enemyStrongImage;
    enemyStrongImage.loadFromFile("images/Enemy_2.png"); // загружаем изображение врага
    Image bonusesImage;
    bonusesImage.loadFromFile("images/bullet.png"); // загружаем изображение бонуса

    Image bulletHeroImage;//изображение для пули
    bulletHeroImage.loadFromFile("images/Hero_bullet.png");//загрузили картинку в объект изображения
    Image bulletCommonImage;//изображение для пули
    bulletCommonImage.loadFromFile("images/Enemy_bullet_1.png");//загрузили картинку в объект изображения
    Image bulletThreeBulletImage;//изображение для пули
    bulletThreeBulletImage.loadFromFile("images/Enemy_bullet_2.png");//загрузили картинку в объект изображения
    Image bulletStrongImage;//изображение для пули
    bulletStrongImage.loadFromFile("images/Enemy_bullet_3.png");//загрузили картинку в объект изображения

    Texture map;//текстура карты
    map.loadFromImage(map_image);//заряжаем текстуру картинкой
    Sprite s_map;//создаём спрайт для карты
    s_map.setTexture(map);//заливаем текстуру спрайтом

    Clock clock;


    PlayerShip player{heroImage, bulletHeroImage, (SCREEN_W - PLAYER_W)/2, SCREEN_H - 10 - PLAYER_H, PLAYER_W, PLAYER_H};//объект класса игрока

    std::list<Bullet*> enBullets; // Список вражеских пуль
    std::list<Bullet*> plBullets; // Список пуль игрока

    std::list<Enemy*> enemies; // Список пуль

    std::list<Bonus*> bonuses; // Список бонусов


    enemies.push_back(new Enemy(enemyCommonImage, bulletCommonImage, ENEMY_W, ENEMY_H, "common"));
    enemies.push_back(new Enemy(enemyCommonImage, bulletCommonImage, ENEMY_W, ENEMY_H, "common"));
    enemies.push_back(new Enemy(enemyCommonImage, bulletCommonImage, ENEMY_W, ENEMY_H, "common"));
    enemies.push_back(new Enemy(enemyCommonImage, bulletCommonImage, ENEMY_W, ENEMY_H, "common"));
    enemies.push_back(new Enemy(enemyCommonImage, bulletCommonImage, ENEMY_W, ENEMY_H, "common"));


    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time = time/1000;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            while (Keyboard::isKeyPressed(Keyboard::P)) // Останавливаем если нажата клавиша P
                clock.restart();
        }

        respawnEnemy(time, enemies, enemyCommonImage, enemyThreeBulletImage, enemyStrongImage,
                     bulletCommonImage, bulletThreeBulletImage, bulletStrongImage);

        spawnBonus(time, bonuses, bonusesImage);

        updateObjects(time, player, plBullets, enemies, enBullets, bonuses);

        collisionCheck(player, plBullets, enemies, enBullets, bonuses);

        window.clear(); // Внимание! Порядок отрисовки важен! Порядок отрисовки: Карта - Пули - Враги - Игрок
        window.draw(s_map); // Рисуем фон

        draw(window, player, plBullets, enemies, enBullets, bonuses);

        window.display();
    }
}
