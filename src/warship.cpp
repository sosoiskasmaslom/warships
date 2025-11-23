
#include "warship.h"
#include "tunnel.h"
using namespace std;

Warship::Warship(string host_,
                 unsigned port_, unsigned n,
                 string name_):
field(n)
{
    conn.set("127.0.0.1", port_);

    if (name_ == "")
    { name_ = string("Player") + to_string(conn.get_host()+1); }
    conn.hello(name_);

    field.filling_ships(3);
    conn.ready();
}

Warship::Warship(string host_, 
                 unsigned port_, unsigned n):
Warship(host_, port_, n, "")
{}

Warship::Warship(string host_, 
                 unsigned port_,
                 string name_):
Warship(host_, port_, 10, name_)
{}

Warship::Warship(string host_, 
                 unsigned port_):
Warship(host_, port_, 10, "")
{}

bool Warship::game()
{
    conn.start();

    Point hehe;
    for (unsigned x, y, i = conn.get_host(); ; ++i)
    {
        cout << field << endl;
        if (i%2)
        {
            cout << "Guess where (x, y): ";
            cin >> x >> y;
            conn.shot(Point(y, x));
            i += conn.result();
        }
        else
        {
            hehe = Point(conn.shot());
            conn.result(hehe, field.attack(hehe));
            i += field.attack(hehe);
        }
    }
}