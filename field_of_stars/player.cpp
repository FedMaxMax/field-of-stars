#include "player.h"

Player::Player(Image &image, int W, int H, std::string Name):Entity(image, (800-W)/2, 630 - H, W, H, Name){
    playerScore = 0;
    state = up;
    speed = 0.3;
    if (name == "Player1"){
        //Задаем спрайту один прямоугольник для
        //вывода одного игрока. IntRect – для приведения типов
        sprite.setTextureRect(IntRect(0, 0, w, h));
    }
}

void Player::control(){
    if (Keyboard::isKeyPressed(Keyboard::A) && Keyboard::isKeyPressed(Keyboard::D)){
       state = stay;
    }
    if (Keyboard::isKeyPressed(Keyboard::A)) {
        state = left;
    } else
    if (Keyboard::isKeyPressed(Keyboard::D)) {
        state = right;
    }else
    {
        state = stay;
    }
}

Bullet *Player::strike(Image& BulletImage)
{
    return new Bullet(BulletImage, x + (w/2) - 16, y - 16, 16, 16, "Bullet");
}

void Player::update(float time) //метод "оживления/обновления" объекта класса.
{
    if (life) {//проверяем, жив ли герой
        control();//функция управления персонажем

        if(state == right){
            dx = speed;
        }else if(state == left){
            dx = -speed;
        }else
            dx = 0;
        }

        x += dx*time; //движение по “X”

        if (x <= 10){
            x -= dx*time;
        }
        if (x + w >= 790){
            x -= dx*time;
        }


        state = stay;
        sprite.setPosition(x, y); //спрайт в позиции (x, y).
        if (health <= 0){
            life = false;
            dx = 0;
        }//если жизней меньше 0, либо равно 0, то умираем
}
