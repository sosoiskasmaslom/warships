
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "jack.h"
#include "tunnel.h"
#include "point.h"
#include "endgame_exception.h"

// unsigned sti(std::string);
// std::string to_hex(unsigned);

// класс кораблей
// будет описывать здоровье, местоположение и размеры кораблся
// так же на его совести попадание в корабль 
// и насколько он живой (ну хп кароч)
class Ship
{
public:
    // тут без векторов и наверн основной метод создания
    // но я добавлю чтоб коорды через вектор задавались
    // и метод чтоб можно было вектор прост закинуть
    // а так это длина(по модулю), х, у и вертикальность
    explicit Ship(unsigned, unsigned, unsigned, bool);
    explicit Ship(std::vector<unsigned>); 
    explicit Ship(unsigned, Point, bool);

    // метод для проверки существует ли корабль в данной точке
    // \param unsigned y
    // \param unsigned х
    bool attack(unsigned, unsigned) const;
    // метод для проверки существует ли корабль в данной точке
    // \param Point
    bool attack(Point) const;
    // просто сносит n хп
    void damage(unsigned);

    const Point& get_coord() const;
    unsigned get_length() const;
    bool get_vertical() const;
    unsigned get_health() const;
    const std::vector<unsigned>& get_ill() const;

    // проверяем живой ли корабль
    bool is_alive() const;

private:
    // местоположение корабля буду задавать как массив
    // голова + длина
    // надо уточнять только его ориентацию
    // вертикаль или горизонталь 

    // координаты корабля (y, x)
    // по своей сути, голова коробля или 1ая клетка
    Point coord; 
    unsigned length; // длина корабля
    // 1 - вертикаль
    // 0 - горизонталь
    bool vertical; // вертикальность
    // вектор длиной равной длине корабля
    // 0 - живой   1 - есть попадание
    unsigned health; // здоровье (числовая величина, ни на что не претендующая)
    // вектор попаданий в корабль
    // хранит зачения отступа от головы
    std::vector<unsigned> ill; 
};

// класс игрового поля (в размере 1 штука)
// ну во первых создание игрового пространства
// отрисовка
// хранение массива кораблей и взаимодействие с ними
class Field
{
public:
    // значение по умолчанию
    explicit Field(); 
    explicit Field(unsigned);
    
    // создание поля из данных строки
    explicit Field(std::string); // нужно для работы с сокетами

    // добавление корабля
    bool add_ship(unsigned, unsigned, unsigned, bool);
    // заполняем поле кораблями
    void filling_ships(); 
    // на случай если кораблей нужно меньше
    void filling_ships(unsigned); 

    // добавление shot на карту
    // \param pos координаты shot'а
    // \param s флаг попадания;
    // 1 - если попал
    void add_try(Point, bool);
    bool check_try(Point);

    // прост перекопирую определение функций из Ship

    // \param y 1-ый аргумент, 2-ая координата точки
    // \param x 2-ой аргумент, 1-ая координата точки
    //
    // ( для подробного описания смотри функцию Field::attack(Point) )
    unsigned attack(unsigned, unsigned) const;
    // в общем функция простая и понятная
    // даешь ей координаты - она выдает ответ
    // - если в эту клетку уже был произведен shot - ответ 2
    // ( кроме случаев попадания по кораблю )
    // - если есть попадание в корабль - ответ 1
    // - если промах - ответ 0
    unsigned attack(Point) const;

    // проверка на наличие живых кораблей
    // 1 если еще есть
    bool is_alive(); 

    // формат инфы о поле в строку
    std::string to_text(); // нужно для работы с сокетами
    // ну в целом в эту строку нужно кинуть только размер поля
    // то есть одно число, ну нормально

    // получение списка короблей
    const std::vector<Ship>& get_ships() const; 
    // получение размера поля
    unsigned get_n() const; 
    Ship& get_ship(Point);

private:
    // ну как бы основное поле
    // просто хранит корабли по которым будет все время пробегаться
    std::vector<Ship> ships; // вектор хранения кораблей
    // массив состояищий из 2-ух векторов хранения shot'ов
    // - 1-ый элемент массива содержит удачные shot'ы ( попавшие в корабли ) 
    // - 2-ой элемент массива содержит все остальные shot'ы
    std::vector<Point> tries[2];
    std::vector<unsigned> templ; // вектор заготовок кораблей, размещаемых на поле
    unsigned n; // размер поля >3
    // пусть будет одномерным массивом
    // потом просто создам какой нить i 
    // который будет монотонно возрастать на 1
};
std::ostream& operator<<(std::ostream&, const Field&);
// капец эта функция тяжелой будет
// ну да ладно, она только для cli версии
std::ostream& print(std::ostream&, std::vector<Field>);


// класс Warship
// олицетворяющий сам игровой процесс
// в его обязанности входит 
// - создание и управление соединением между игроками
// - создание и управление игровыми полями, заполнение их кораблями
// - фиксирование выстрелов и попаданий
//
// на войне как на войне, а на войне как на войне
class Warship
{
public:
    // конструктор класса
    // устанавливает подключение между игроками
    // и создает игровое поле
    // \param host_ ip аддрес либо invite-ссылка
    // \param port_ используемый порт 
    // \param n размер поля
    // \param name_ имя игрока
    Warship(std::string, unsigned, unsigned, std::string);
    Warship(std::string, unsigned, unsigned);
    Warship(std::string, unsigned, std::string);
    Warship(std::string, unsigned);

    // функция окончания игры победой
    // ( не в смысле вызвал - выиграл )
    bool win();
    // функция окончания игры поражением
    // ( не в смысле вызвал - проиграл )
    bool lose();

    // основная игровая сессия
    bool game();

protected:
    sTunnel conn;
    std::vector<Field> fields {Field(), Field()};
    std::vector<Point> shots;
};

// 13 октября 2025 года
// С днем рождения, Серега