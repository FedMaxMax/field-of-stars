#include <SFML/Graphics.hpp>
#include <list>
#include <ctime>
#include "global.h"
#include "playership.h"
#include "entity.h"
#include "enemy.h"
#include "bullet.h"

using namespace sf;

int main()
{
    srand(time(0));
    VideoMode desktop = sf::VideoMode::getDesktopMode();
    RenderWindow window(sf::VideoMode(SCREEN_W, SCREEN_H, desktop.bitsPerPixel), "Field of stars");

    Font font;//шрифт
    font.loadFromFile("CyrilicOld.ttf");//передаем нашему шрифту файл шрифта
    Text text("", font, 20);//создаем объект текст
    text.setColor(Color::Red);//покрасили текст в красный
    text.setStyle(Text::Bold);//жирный текст.

    Image map_image;//объект изображения для карты
    map_image.loadFromFile("images/fon.png");//загружаем файл для карты
    Texture map;//текстура карты

    map.loadFromImage(map_image);//заряжаем текстуру картинкой
    Sprite s_map;//создаём спрайт для карты
    s_map.setTexture(map);//заливаем текстуру спрайтом

    Clock clock;

    Image heroImage;
    heroImage.loadFromFile("images/spaceship.png"); // загружаем изображение игрока
    Image enemyImage;
    enemyImage.loadFromFile("images/enemyspace.png"); // загружаем изображение врага

    Image bulletImage;//изображение для пули
    bulletImage.loadFromFile("images/plazmabullet.png");//загрузили картинку в объект изображения
    PlayerShip p{heroImage, bulletImage, (SCREEN_W - 96)/2, SCREEN_H - 10 - 96, 96, 96};//объект класса игрока

    std::list<Bullet*> enBullets; // Список вражеских пуль
    std::list<Bullet*> plBullets; // Список пуль игрока
    std::list<Bullet*>::iterator itBul;

    std::list<Enemy*> enemies; // Список пуль
    std::list<Enemy*>::iterator itEn;

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

        for (itBul = enBullets.begin(); itBul != enBullets.end(); itBul++) // Обновляем пули. Должно быть перед обновлением врагов
                                                                       // и игрока, так как они могут создать новые!
        {
            (*itBul)->update();
            (*itBul)->move(time);
        }

        for (itBul = plBullets.begin(); itBul != plBullets.end(); itBul++)
        {
            (*itBul)->update();
            (*itBul)->move(time);
        }

        p.update(); // Обновляем игрока
        p.move(time);
        p.increaseShootTimer(time, plBullets);

        for(itEn = enemies.begin(); itEn != enemies.end(); itEn++) // Обновляем врагов
        {
            (*itEn)->update();
            (*itEn)->move(time);
            (*itEn)->increaseShootTimer(time, enBullets);
        }

        for(itBul = enBullets.begin(); itBul != enBullets.end(); itBul++) // Проверяем столкновение пуль с игроком
        {
            if(p.getRect().intersects((*itBul)->getRect()))
            {
                p.getDamaged((*itBul)->getCollisionDamage());
                (*itBul)->die();
            }
        }

        for(itBul = plBullets.begin(); itBul != plBullets.end(); itBul++) // Проверяем столкновение пуль с врагами
        {
            if((*itBul)->isAlive())
            {
                for(itEn = enemies.begin(); itEn != enemies.end() && (*itBul)->isAlive(); itEn++)
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

        window.clear(); // Внимание! Порядок отрисовки важен! Порядок отрисовки: Карта - Пули - Враги - Игрок
        window.draw(s_map); // Рисуем фон

        for(itBul = enBullets.begin(); itBul != enBullets.end(); itBul++) // Рисуем вражеские пули (удаляем если уничтожены)
        {
            if((*itBul)->isAlive())
            {
                window.draw(*((*itBul)->getSprite()));
            } else {
                delete *itBul;
                enBullets.erase(itBul);
            }
        }

        for(itBul = plBullets.begin(); itBul != plBullets.end(); itBul++) // Рисуем пули игрока (удаляем если уничтожены)
        {
            if((*itBul)->isAlive())
            {
                window.draw(*((*itBul)->getSprite()));
            } else {
                delete *itBul;
                plBullets.erase(itBul);
            }

        }

        for(itEn = enemies.begin(); itEn != enemies.end(); itEn++) // Рисуем врагов (удаляем если уничтожены)
        {
            if((*itEn)->isAlive())
            {
                window.draw(*((*itEn)->getSprite()));
            } else {
                delete *itEn;
                enemies.erase(itEn);
            }
        }

        window.draw(*(p.getSprite())); // Рисуем игрока

        window.display();
    }
}

