
#include <iostream>
#include <vector>
#include <string>
#include <map>
using namespace std;

class Ship
{
friend ostream& operator<<(ostream&, const Ship&);

public:
    // х у размер и наскок вертикален
    explicit Ship(int, int, int, bool);

    const vector<int>& get_pos() const;
    const vector<int>& get_damage() const;
    vector<int>& get_damage();

    int get_size() const;

    // проверка на попадание
    // возвращает 1 если попал
    bool attack(vector<int>);
    bool attack(int, int);

private:
    vector<int> coordinates;
    vector<int> damage;
    int size;
    bool hor; // 1 - вертикален; 0 - горизонтален
};

class Field
{
friend ostream& operator<<(ostream&, const Field&);

public:
    explicit Field(); // поле по умолчанию (10)
    explicit Field(int); // передаем размер поля
    // поле квадратное если что

    int get_size() const;
    const vector<vector<int>>& get_pos() const;

    // так ну х у размер и каскок вертикален
    void add_ship(int, int, int, bool);
    void add_ship(Ship);
    const Ship& get_back() const;

    bool attack(vector<int>) const;
    bool attack(int, int) const;
    
private:
    int size;
    vector<vector<int>> coordinates;
    vector<Ship> ships;
};

class Warship
{
public:
    Warship();
    explicit Warship(int);
    explicit Warship(bool, int);

    class TooSmallFieldError;
    class ShipExist;
    class NetworkError;

    bool sync();
    
    // for tests
    void print_ships(const Ship&) const;

private:
    Field& create_field(Field& other);
    
    bool online; // будут режимы игры сингл 0 и по сети 1
    int size; // размер поля
    // данные выше для обоих игроков должны быть одинаковыми
    // в смысле когда по сети

    Field good_field; // ну очевидно, свое поле, родное
    Field bad_field;  // и поле врага, вонючее

};

