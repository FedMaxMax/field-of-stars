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

Font BattleScreen::createFont()
{
    Font font;//шрифт
    font.loadFromFile("CyrilicOld.ttf");//передаем нашему шрифту файл шрифта
    Text text("", font, 20);//создаем объект текст
    text.setColor(Color::Red);//покрасили текст в красный
    text.setStyle(Text::Bold);//жирный текст.
    return font;
}


void BattleScreen::updateObjects(float& p_time, PlayerShip& p_player, std::list<Bullet*>& p_plBullets, std::list<Enemy*>& p_enemies, std::list<Bullet*>& p_enBullets)
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
}

void BattleScreen::collisionCheck(PlayerShip& p_player, std::list<Bullet*>& p_plBullets, std::list<Enemy*>& p_enemies, std::list<Bullet*>& p_enBullets)
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

void BattleScreen::draw(RenderWindow& p_window,PlayerShip& p_player, std::list<Bullet*>& p_plBullets, std::list<Enemy*>& p_enemies, std::list<Bullet*>& p_enBullets)
{
    std::list<Bullet*>::iterator itBul;
    std::list<Enemy*>::iterator itEn;
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

void BattleScreen::play()
{

    srand(time(0));
    VideoMode desktop = sf::VideoMode::getDesktopMode();
    RenderWindow window(sf::VideoMode(SCREEN_W, SCREEN_H, desktop.bitsPerPixel), "Field of stars");

    Font font = createFont();//шрифт

    Image map_image;//объект изображения для карты
    map_image.loadFromFile("images/fon.png");//загружаем файл для карты
    Image heroImage;
    heroImage.loadFromFile("images/spaceship.png"); // загружаем изображение игрока
    Image enemyImage;
    enemyImage.loadFromFile("images/enemyspace.png"); // загружаем изображение врага
    Image bulletImage;//изображение для пули
    bulletImage.loadFromFile("images/plazmabullet.png");//загрузили картинку в объект изображения

    Texture map;//текстура карты
    map.loadFromImage(map_image);//заряжаем текстуру картинкой
    Sprite s_map;//создаём спрайт для карты
    s_map.setTexture(map);//заливаем текстуру спрайтом

    Clock clock;


    PlayerShip player{heroImage, bulletImage, (SCREEN_W - 96)/2, SCREEN_H - 10 - 96, 96, 96};//объект класса игрока

    std::list<Bullet*> enBullets; // Список вражеских пуль
    std::list<Bullet*> plBullets; // Список пуль игрока

    std::list<Enemy*> enemies; // Список пуль


    enemies.push_back(new Enemy(enemyImage, bulletImage, 96, 96, "common"));
    enemies.push_back(new Enemy(enemyImage, bulletImage, 96, 96, "strong"));
    enemies.push_back(new Enemy(enemyImage, bulletImage, 96, 96, "threebullet"));

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

        updateObjects(time, player, plBullets, enemies, enBullets);

        collisionCheck(player, plBullets, enemies, enBullets);

        window.clear(); // Внимание! Порядок отрисовки важен! Порядок отрисовки: Карта - Пули - Враги - Игрок
        window.draw(s_map); // Рисуем фон

        draw(window, player, plBullets, enemies, enBullets);

        window.display();
    }
}
