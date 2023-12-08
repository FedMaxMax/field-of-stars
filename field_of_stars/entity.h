#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>

using namespace sf;

////////////////////////////КЛАСС СУЩНОСТЬ////////////////////////
class Entity {
public:
    enum { left, right, up, down, stay} state;// тип перечисления - состояние объекта
    float dx, dy, x, y, speed, moveTimer;//добавили переменную таймер для будущих целей
    int w, h, health; //переменная “health”, хранящая жизни игрока
    bool life; //переменная “life” жизнь, логическая
    Texture texture;//сфмл текстура
    Sprite sprite;//сфмл спрайт
    std::string name;//враги могут быть разные, врагов можно различать по именам

    Entity(Image &image, float X, float Y, int W, int H, std::string Name);
    //каждому можно дать свое действие в update() в зависимости от имени
    virtual void update(float time) = 0;
    FloatRect getRect();
};

#endif // ENTITY_H
