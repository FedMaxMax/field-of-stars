#ifndef TEXTWINDOW_H_
#define TEXTWINDOW_H_

#include <stdint.h>
#include <SFML/Graphics.hpp>

//Класс, позволяющий вывести текст с фоном на экран

class TextWindow
{
private:
    sf::Text m_title;   //Текст для вывода
    sf::RectangleShape m_background;//Фон для вывода
public:
    //Конструктор класса
    TextWindow(uint32_t p_x,    //Координата x для обозначения места размещения текста и фона
               uint32_t p_y,    //Координата y для обозначения места размещения текста и фона
               std::string p_title, //Текст для вывода
               uint8_t p_size,      //Ширина текста
               sf::Color p_color,   //Цвет текста
               uint32_t p_width,    //Ширина фона
               uint32_t p_height,   //Высота фона
               sf::Color p_colorBackground);//Цвет фона
    void editTitle(std::string p_title);    //Метод, изменяющий текст
    void draw(sf::RenderWindow& p_window);  //Метод, выводящий фон и текст на экран
    void setFont(sf::Font &p_font);         //Метод, задающий шрифт текста
};

#endif // TEXTWINDOW_H
