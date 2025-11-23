
#include "warship.h"
#include "alg.cpp"
using namespace std;


Point::Point()
{}

Point::Point(unsigned y_, unsigned x_):
y(y_), x(x_)
{}

Point::Point(string ss):
Point(sti(ss.substr(1, 1)), sti(ss.substr(0, 1)))
{}

unsigned Point::get_x() const
{ return x; }

unsigned Point::get_y() const
{ return y; }

Point Point::get() const
{ return *this; }

void Point::set(unsigned y_, unsigned x_)
{ y = y_; x = x_; }

unsigned Point::operator[](unsigned i) const
{ 
    if (i>1) throw runtime_error("Out from Point size");
    if (i == 1) { return x; }
    else { return y; }
}

unsigned& Point::operator[](unsigned i)
{ 
    if (i>1) throw runtime_error("Out from Point size");
    if (i == 1) { return x; }
    else { return y; }
}

Point& Point::operator=(const Point& another)
{
    set(another.get_y(), another.get_x());
    return *this;
}

string Point::to_string() const
{ return to_hex(x)+to_hex(y); }

std::ostream& operator<<(std::ostream& out, const Point& other)
{ return out << other.to_string(); }


Ship::Ship(unsigned length, unsigned y, unsigned x, bool vertical):
Ship(length, Point{y, x}, vertical)
{}

Ship::Ship(vector<unsigned> templ):
Ship(templ[0], Point {templ[1], templ[2]}, templ[3])
{}

Ship::Ship(unsigned length, Point coord, bool vertical):
coord(coord), length(length), vertical(!vertical), health(length)
{}

bool Ship::attack(unsigned x, unsigned y) const
{ return attack(Point {y, x}); };

bool Ship::attack(Point pos) const
{
    // проверка жив ли корабль в принципе
    if (!health) 
    { return 0; }
    // проверка находится ли выстрел на одной линии с головой 
    if (pos[1-vertical] != coord[1-vertical]) 
    { return 0; }
    // фиксирование попадания диапозоном
    if 
    (
        !(
            (coord[vertical] <= pos[vertical]) && \
            (pos[vertical] <= coord[vertical]+length-1)
        )
    ) 
    { return 0; }
    return 1;
}

const Point& Ship::get_coord() const
{ return coord; }

unsigned Ship::get_length() const
{ return length; }

bool Ship::get_vertical() const
{ return vertical; }

unsigned Ship::get_health() const
{ return health; }

const vector<unsigned>& Ship::get_ill() const
{ return ill; }

bool Ship::is_alive() const
{ return (health>0); }


Field::Field(): 
Field(10)
{}

Field::Field(unsigned n): 
n(max(n, (unsigned) 3)), 
templ({4, 3, 3, 2, 2, 2, 1, 1, 1, 1})
{}

bool Field::add_ship(unsigned lenght, unsigned y, unsigned x, bool vertical)
{ 
    if 
    (
        x >= n || y >= n || \
        (x+lenght-1 >= n && !vertical) || (y+lenght-1 >= n and vertical)
    ) 
    { return 0; }

    for(int j = 0; j<lenght; ++j)
    { 
        if 
        (
            ( vertical && attack(y+j, x)) || \
            (!vertical && attack(y, x+j))
        )
        { return 0; }
    }

    ships.push_back(Ship(lenght, Point{y, x}, vertical));
    return 1;
}

void Field::filling_ships()
{ filling_ships(size(templ)); }

void Field::filling_ships(unsigned count)
{
    unsigned x, y, vertical;
    if (count > size(templ)) return;
    while (!ships.empty()) { ships.pop_back(); }
    for (int i = 0; ships.size() < count; i++)
    {
        cout << "Enter a coord and vertical for ship " << templ[i] << ": ";
        cin >> x >> y >> vertical;        
        if (!add_ship(templ[i], y, x, vertical)) 
        { --i; }
    }
}

void Field::add_try(Point other)
{
    
}

unsigned Field::attack(unsigned y, unsigned x) const
{ return attack(Point {y, x}); }

unsigned Field::attack(Point pos) const
{
    for (auto ship: ships)
    { 
        if (ship.attack(pos)) 
        { return 1; }; 
    }
    return 0;
}

bool Field::is_alive()
{
    for (auto ship: ships)
    { if (ship.is_alive()) { return 1; }; }
    return 0;
}

string Field::to_text()
{ return to_string(n); }

const vector<Ship>& Field::get_ships() const
{ return ships; }

unsigned Field::get_n() const
{ return n; }

ostream& operator<<(ostream& out, const Field& other)
{
    for(int i = 0; i < other.get_n(); ++i)
    {
        for(int j = 0; j < other.get_n(); ++j)
        { out << ( (other.attack(i, j)) ? "X " : "_ " ); }
        out << endl;
    }
    return out;
}
