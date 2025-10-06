
#include "warship.h"

/* кораблики урааа */
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

ostream& operator<<(ostream& out, const Ship& other)
{
    out << other.coordinates[0] << ' '\
        << other.coordinates[1] << ' ' << other.get_size(); 
    return out;
}


/* по полям, по полям... */
Field::Field(): Field(10)
{}

Field::Field(int size): size(size)
{}

int Field::get_size() const
{ return size; }

const vector<vector<int>>& Field::get_pos() const
{ return coordinates; }

void Field::add_ship(int x, int y, int size, bool hor)
{ add_ship(Ship(x, y, size, hor)); }

void Field::add_ship(Ship other)
{ ships.push_back(other); }

const Ship& Field::get_back() const
{ return ships[ships.size()-1]; }

bool Field::attack(vector<int> pos) const
{
    int flag = 0;
    for (auto ship: ships)
    { if (ship.attack(pos)) { flag++; } }
    //coordinates.push_back(vector<int>{pos[0], pos[1], flag});
    return flag;
}

bool Field::attack(int x, int y) const
{ return attack(vector<int> {x, y}); }

// ostream& operator<<(ostream& out, const Field& other)
// {
//     for (int i = 0; i < other.get_size(); ++i)
//     {
//         for (int j = 0; j < other.get_size(); ++j)
//         { out << ((other.attack(j, i)) ? 1 : 0) << ' '; }
//     }
//     return out;
// }


/* на войне как на войне, а на войне как на войне */
Warship::Warship(): Warship(0, 10)
{}

Warship::Warship(int n): Warship(0, n)
{}

Warship::Warship(bool online, int size):
online(online), size(size), good_field(size)
{
    if (size<3) throw TooSmallFieldError{size};
    if (online) 
    {}
    
    create_field(good_field);
    if (!online) create_field(bad_field);
}

Field& Warship::create_field(Field& other)
{
    vector<int> templ_ship {4, 3, 3, 2, 2, 2, 1, 1, 1, 1};
    vector<Ship> fuck
    {
        Ship{1, 1, 1, 1},
        Ship{1, 1, 1, 1},
        Ship{1, 1, 1, 1},
        Ship{1, 1, 1, 1},
        Ship{1, 1, 1, 1},
        Ship{1, 1, 1, 1},
        Ship{1, 1, 1, 1},
        Ship{1, 1, 1, 1},
        Ship{1, 1, 1, 1},
        Ship{3, 6, 5, 1},
    };
    for (auto templ: fuck)
    { other.add_ship(templ); }
    // for (auto templ: templ_ship)
    // {
    //     int x, y, hor;
    //     cout << "Enter ship " << templ << ": ";
    //     cin >> x >> y >> hor;
    //     other.add_ship(x, y, templ, hor);
    // }   
    return other;
}


class Warship::TooSmallFieldError
{
public:
    TooSmallFieldError(int n): size(n) {}

    string get_Message() const
    { return "A field size of " + to_string(size) + " is too small"; }
private:
    int size;
};