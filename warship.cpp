
#include "warship.h"
#include "tunnel.h"
using namespace std;

Warship::Warship(string host_,
                 unsigned port_, unsigned n,
                 string name_)
{
    conn.set("127.0.0.1", port_);

    if (name_ == "")
    { name_ = string("Player") + to_string(conn.get_host()+1); }
    conn.hello(name_);

    fields[0].filling_ships(3);
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
    bool s;
    for (unsigned heh, x, y, i = conn.get_host(); ; ++i)
    {
        if (!fields[0].is_alive())
        { throw EndGame(conn[1]); }
        
        print(cout, fields);
        for (auto ship: fields[0].get_ships())
        { cout << ship.get_health() << ' '; }
        cout << endl << endl;
        if (i%2)
        {
            for (;;)
            { 
                cout << "Guess where (x, y): ";
                cin >> x >> y; 
                if 
                (
                    !fields[1].check_try(Point(y, x)) && \
                    x < fields[0].get_n() && y < fields[0].get_n()
                )
                { break; }
                cout << "WRONG" << endl;
            }
            conn.shot(Point(y, x));
            s = conn.result();
            i += s;
            fields[1].add_try(Point(conn.get_shot()), s);
        }
        else
        {
            hehe = Point(conn.shot());
            conn.result(hehe, fields[0].attack(hehe));
            if (fields[0].attack(hehe))
            {
                ++i;
                fields[0].get_ship(hehe).damage(1);
            }            
        }
    }
    return 1;
}