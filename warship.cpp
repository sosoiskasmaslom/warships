
#include <iostream>
#include <vector>
#include <string>
#include <map>
using namespace std;


/* кораблики урааа*/
class Ship
{
public:
    explicit Ship(int, int, int, bool);

    const vector<int>& get_pos() const;
    int get_size() const;

    bool attack(vector<int>);
    bool attack(int, int);

private:
    vector<int> coordinates;
    int size;
    bool hor; // 1 - vertical; 0 - horizontal
};

Ship::Ship(int x, int y, int size, bool hor):
coordinates({x, y}), size(size), hor(hor)
{}

const vector<int>& Ship::get_pos() const
{ return coordinates; }

int Ship::get_size() const
{ return size; }

bool Ship::attack(vector<int> pos)
{
    if (pos[1-hor] != coordinates[1-hor]) return 0;
    if (!((coordinates[hor] <= pos[hor]) && (pos[hor] <= coordinates[hor]+size))) return 0;
    return 1;
}

bool Ship::attack(int x, int y)
{ return attack(vector<int>{x, y}); }


/* по полям, по полям... */
class Field
{
friend ostream& operator<<(ostream&, const Field&);

public:
    explicit Field();
    explicit Field(int);

    int get_size() const;
    const vector<vector<int>>& get_pos() const;

    void add_ship(int, int, int, bool);
    bool attack(vector<int>);
    bool attack(int, int);
    
private:
    int size;
    vector<vector<int>> coordinates;
    vector<Ship> ships;
};

Field::Field(): Field(10)
{}

Field::Field(int size): size(size)
{}

int Field::get_size() const
{ return size; }

const vector<vector<int>>& Field::get_pos() const
{ return coordinates; }

void Field::add_ship(int x, int y, int size, bool hor)
{ ships.push_back(Ship(x, y, size, hor)); }

bool Field::attack(vector<int> pos)
{
    int flag = 0;
    for (auto ship: ships)
    { if (ship.attack(pos)) { flag++; } }
    coordinates.push_back(vector<int>{pos[0], pos[1], flag});
    return flag;
}

bool Field::attack(int x, int y)
{ return attack(vector<int> {x, y}); }


/* на войне как на войне, а на войне как на войне */
class Warship
{
public:
    Warship();
    explicit Warship(int);

    class TooSmallFieldError;

    // for tests
    // map<string, int>& show_ships()
    // { return ships; }
    // vector<vector<int>>& show_field()
    // { return field; }

private:
    void create_field();

    Field good_field; // ну очевидно, свое поле, родное
    Field bad_field;  // и поле врага, вонючее

    bool online; // будут режимы игры сингл и по сети
    
};

