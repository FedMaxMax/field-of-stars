#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <list>
#include "global.h"
#include "entity.h"
#include "player.h"
#include "entity.h"
#include "enemy.h"
#include "bullet.h"

using namespace sf;

int main()
{
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode(SCREEN_W, SCREEN_H, desktop.bitsPerPixel), "Field of stars");

    Font font;//шрифт
    font.loadFromFile("CyrilicOld.ttf");//передаем нашему шрифту файл шрифта
    Text text("", font, 20);//создаем объект текст
    text.setColor(Color::Red);//покрасили текст в красный
    text.setStyle(Text::Bold);//жирный текст.

    Image map_image;//объект изображения для карты
    map_image.loadFromFile("images/map_new.png");//загружаем файл для карты
    Texture map;//текстура карты

    map.loadFromImage(map_image);//заряжаем текстуру картинкой
    Sprite s_map;//создаём спрайт для карты
    s_map.setTexture(map);//заливаем текстуру спрайтом

    Clock clock;

    Image heroImage;
    heroImage.loadFromFile("images/hero.png"); // загружаем изображение игрока
    Image easyEnemyImage;
    easyEnemyImage.loadFromFile("images/enemy.png"); // загружаем изображение врага

    Image BulletImage;//изображение для пули
    BulletImage.loadFromFile("images/bullet.png");//загрузили картинку в объект изображения
    Player p(heroImage, 96, 96, "Player1");//объект класса игрока

    std::list<Entity*> enemies; //список врагов
    std::list<Entity*> Bullets; //список пуль
    std::list<Entity*> enemyBullets; //список вражеских пуль
    std::list<Entity*>::iterator it; //итератор чтобы проходить по элементам списка
    std::list<Entity*>::iterator it2; //итератор чтобы проходить по элементам списка
    Enemy *pointEnemy;

    int enemiesCount = 0; //текущее количество врагов в игре

    //Заполняем список объектами врагами
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        float xr = 150 + rand() % 500; // случайная координата врага на поле игры по оси “x”
        float yr = 150 + rand() % 350; // случайная координата врага на поле игры по оси “y”
        //создаем врагов и помещаем в список
        enemies.push_back(new Enemy(easyEnemyImage, xr, yr, 96, 96, "EasyEnemy"));
        enemiesCount += 1; //увеличили счётчик врагов
    }

    int createObjectForMapTimer = 0;//Переменная под время для генерирования камней
    int shotTimer = 0;
    int enemiesTimer = 0;

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time = time / 800;

        createObjectForMapTimer += time;//наращиваем таймер
        shotTimer += time;

        if (createObjectForMapTimer>3000){
            createObjectForMapTimer = 0;//обнуляем таймер
        }

        if(enemiesCount < ENEMY_COUNT){
            enemiesTimer += time;
            if(enemiesTimer > 300){
                float xr = 150 + rand() % 500; // случайная координата врага на поле игры по оси “x”
                float yr = 150 + rand() % 350; // случайная координата врага на поле игры по оси “y”
                //создаем врагов и помещаем в список
                enemies.push_back(new Enemy(easyEnemyImage, xr, yr, 96, 96, "EasyEnemy"));
                enemiesCount += 1; //увеличили счётчик врагов
                enemiesTimer = 0;
            }
        }

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            //стреляем по нажатию клавиши "P"
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::P && shotTimer > 200)
                {   
                    Bullets.push_back(p.strike(BulletImage));
                    shotTimer = 0;
                }
            }
        }
        p.update(time); //оживляем объект “p” класса “Player”

        //оживляем врагов
        for (it = enemies.begin(); it != enemies.end(); it++)
        {
            (*it)->update(time); //запускаем метод update()
            pointEnemy = ((Enemy*)(*it));
            if (pointEnemy->enemiesBulletsTimer > 2000)
                {
                pointEnemy->enemiesBulletsTimer = 0;
                enemyBullets.push_back(pointEnemy->strike(BulletImage));
                }
        }

        //оживляем пули
        for (it = Bullets.begin(); it != Bullets.end(); it++)
        {
            (*it)->update(time); //запускаем метод update()
        }
        for (it = enemyBullets.begin(); it != enemyBullets.end(); it++)
        {
            (*it)->update(time); //запускаем метод update()
        }

        //Проверяем список на наличие "мертвых" пуль и удаляем их
        for (it = Bullets.begin(); it != Bullets.end(); )//говорим что проходимся от начала до конца
        {// если этот объект мертв, то удаляем его
            if ((*it)-> life == false) { it = Bullets.erase(it); }
            else it++;//и идем курсором (итератором) к след объекту.
        }

        //Проверяем список на наличие "мертвых" вражеских пуль и удаляем их
        for (it = enemyBullets.begin(); it != enemyBullets.end(); )//говорим что проходимся от начала до конца
        {// если этот объект мертв, то удаляем его
            if ((*it)-> life == false) { it = enemyBullets.erase(it); }
            else it++;//и идем курсором (итератором) к след объекту.
        }

        //Проверка пересечения игрока с врагами
        //Если пересечение произошло, то "health = 0", игрок обездвижевается и
        //выводится сообщение "you are lose"
        if (p.life == true){//если игрок жив
            for (it = enemies.begin(); it != enemies.end(); it++){//бежим по списку врагов
                if ((p.getRect().intersects((*it)->getRect())) && ((*it)->name == "EasyEnemy"))
                {
                    p.health = 0;
                    std::cout << "you are lose";
                }
            }
        }
        //Проверка пересечения пуль с врагами
        //Если пересечение произошло, то "health = 0", игрок обездвижевается и
        //выводится сообщение "you are lose"
        for ( it2 = Bullets.begin(); it2 != Bullets.end(); it2++){//бежим по списку пуль
            for (it = enemies.begin(); it != enemies.end(); it++){//бежим по списку врагов
                if (((*it)->getRect().intersects((*it2)->getRect())) && ((*it)->name == "EasyEnemy") && ((*it2)->name == "Bullet"))
                {
                    Bullets.erase(it2);
                    enemies.erase(it);
                    enemiesCount--;
                }
            }
       }

        //Проверка пересечения вражеских пуль с игроком
        //Если пересечение произошло, то "health = 0", игрок обездвижевается и
        //выводится сообщение "you are lose"
        if (p.life == true){//если игрок жив
            for (it = enemyBullets.begin(); it != enemyBullets.end(); it++){//бежим по списку врагов
                if (p.getRect().intersects((*it)->getRect()))
                {
                    p.health = 0;
                    std::cout << "you are lose";
                }
            }
        }
        window.clear();
        /////////////////////////////Рисуем карту/////////////////////
        //объявили переменную здоровья и времени
        window.draw(p.sprite);//рисуем спрайт объекта “p” класса “Player”
        //рисуем врагов
        for (it = enemies.begin(); it != enemies.end(); it++)
        {
            if ((*it)->life) //если враги живы
                window.draw((*it)->sprite); //рисуем
        }
        //рисуем пули
        for (it = Bullets.begin(); it != Bullets.end(); it++)
        {
            if ((*it)->life) //если пули живы
                window.draw((*it)->sprite); //рисуем объекты
        }
        for (it = enemyBullets.begin(); it != enemyBullets.end(); it++)
        {
            if ((*it)->life) //если пули живы
                window.draw((*it)->sprite); //рисуем объекты
        }
        window.display();
    }
    return 0;
}
