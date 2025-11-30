
#pragma once
#include <iostream>

// данный класс вынуждено вынесен в отдельный файл
// дабы избежать ошибок при компиляции

// реализацию класса оставлю в for_warship.cpp 

// спешно добавленный класс координат
// введенный для удобства
// (можно было бы ввести его раньше)
class Point
{
public:
    Point(); // конструктор для последующего определения yx
    Point(unsigned, unsigned); // конструктор yx для обычной точки поля
    Point(std::string); // конструктор для преобразования строки в координату

    unsigned get_x() const; // получение х
    unsigned get_y() const; // получение у
    Point get() const; // получение вектора {y, x}

    void set(unsigned, unsigned); // переназначение yx
    unsigned operator[](unsigned) const;
    unsigned& operator[](unsigned);
    Point& operator=(const Point&);
    bool operator==(const Point&);

    std::string to_string() const;

private:
    unsigned x, y; // координата точки 
};
std::ostream& operator<<(std::ostream&, const Point&);
