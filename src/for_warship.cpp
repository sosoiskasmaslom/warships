
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

bool Point::operator==(const Point& another)
{
    if (get_y() == another.get_y() && get_x() == another.get_x())
    { return 1; }
    return 0;
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

void Ship::damage(unsigned n = 1)
{ health -= n; }

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

void Field::add_try(Point pos, bool s)
{ tries[1-s].push_back(pos); }

bool Field::check_try(Point pos)
{
    for (int i = 0; i<2; ++i)
    {
        for (auto s: tries[i])
        {
            if (s==pos)
            { return 1; }
        }
    }
    return 0;
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
    for (unsigned i = 0; i<2; ++i)
    {
        for (auto tr: tries[i])
        {
            if (pos == tr)
            { return i+2; }
        }
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

Ship& Field::get_ship(Point pos)
{ 
    for (int i = 0; i<ships.size(); ++i)
    { 
        if (ships[i].attack(pos)) 
        { return ships[i]; }
    }
    throw runtime_error("Field::get_ship() - here is not any ships");
}

ostream& operator<<(ostream& out, const Field& other)
{
    print(out, vector<Field>{other});
    return out;
}

ostream& print(ostream& out, vector<Field> fields)
{
    if (!fields.size())
    { throw runtime_error("print() - fields is empty"); }
    for (int k = 0; k < fields.size(); ++k)
    {
        cout << "  ";
        for(int i = 0; i < fields[0].get_n(); ++i)
        { cout << i << ' '; }
        for(int i = 0; i < 3; ++i)
        { cout << ' '; }
    }
    cout << endl;
    for(int i = 0; i < fields[0].get_n(); ++i)
    {
        for (int k = 0; k < fields.size(); ++k)
        {
            { cout << i << ' '; }
            for(int j = 0; j < fields[k].get_n(); ++j)
            {
                switch (fields[k].attack(i, j))
                {
                case 0:
                    out << "_ ";
                    break;
                case 1:
                    out << "■ ";
                    break;
                case 2:
                    out << "X ";
                    break;
                case 3:
                    out << "□ ";
                    break;
                }
            }
            for (int j = 0; j<3; ++j)
            { out << " ";}
        }
        out << endl;
    }
    return out;
}
