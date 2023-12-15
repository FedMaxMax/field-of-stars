#include "enemy.h"

Enemy::Enemy(Image &image, float X, float Y, int W, int H, std::string Name) :Entity(image, X,
                                                                              Y, W, H, Name){
    if (name == "EasyEnemy"){
        //Задаем спрайту один прямоугольник для
        //вывода одного игрока. IntRect – для приведения типов
        sprite.setTextureRect(IntRect(0, 0, w, h));
        direction = rand() % (3); //Направление движения врага задаём случайным образом
        //через генератор случайных чисел
        speed = 0.1;//даем скорость.этот объект всегда двигается
        dx = speed;
        enemiesBulletsTimer = (rand() % 1000);
    }
}

void Enemy::checkCollisionWithBounds()//ф-ция проверки столкновений с картой
{
    if (x <= 10){
        x = 15;
        dx = 0.1;
        direction = rand() % (3);
    }
    if (y <= 10){
        y = 15;
        dy = 0.1;
        direction = rand() % (3);
    }
    if (x + w >= 790){
        x = 785 - w;
        dx = -0.1;
        direction = rand() % (3);
    }
    if (y + h >= 430){
        y = 425 - h;
        dy = -0.1;
        direction = rand() % (3);
    }
}

Bullet *Enemy::strike(Image& BulletImage)
{
    return new Bullet(BulletImage, x + (w/2) - 16, y - 16, 16, 16, "Bullet", down);
}

void Enemy::update(float time)
{
    if (name == "EasyEnemy"){//для персонажа с таким именем логика будет такой
        if (life) {//проверяем, жив ли герой
            switch (direction)//делаются различные действия в зависимости от состояния
            {
            case 0:{//состояние идти вправо
                dx = speed;
                CurrentFrame += 0.005*time;
                if (CurrentFrame > 3) CurrentFrame -= 3;
                sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 0, 96, 96));
                break;
            }
            case 1:{//состояние идти влево
                dx = -speed;
                CurrentFrame += 0.005*time;
                if (CurrentFrame > 3) CurrentFrame -= 3;
                sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 0, 96, 96));
                break;
            }
            case 2:{//идти вверх
                dy = -speed;
                CurrentFrame += 0.005*time;
                if (CurrentFrame > 3) CurrentFrame -= 3;
                sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 0, 96, 96));
                break;
            }
            case 3:{//идти вниз
                dy = speed;
                CurrentFrame += 0.005*time;
                if (CurrentFrame > 3) CurrentFrame -= 3;
                sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 0, 96, 96));
                break;
            }
            }
            enemiesBulletsTimer += (time + ((float)(rand() % 100))/100);
            x += dx*time; //движение по “X”
            //обрабатываем столкновение по Х
            y += dy*time; //движение по “Y”
            checkCollisionWithBounds();//обрабатываем столкновение по Y
            sprite.setPosition(x, y); //спрайт в позиции (x, y).
            if (health <= 0){ life = false; }//если жизней меньше 0, либо равно 0, то умираем
        }
    }
}
