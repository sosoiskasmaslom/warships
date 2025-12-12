
#include "warship.h"
#include "tunnel.h"
#include <future>
#include <QMetaObject>
using namespace std;

Warship::Warship(string host_,
                 unsigned port_, unsigned n,
                 string name_)
{
    conn.set(host_, port_);

    if (name_ == "")
    { name_ = string("Player") + to_string(2-conn.get_host()); }
    conn.hello(name_);

    fields[0].filling_ships(1);
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

bool Warship::logic()
{
    conn.start();

    QObject::connect(this, &Warship::closeGame, [&](){
        conn.bue();
    });

    Point hehe;
    bool s;
    for (unsigned heh, x, y, i = conn.get_host(); ; ++i)
    {
        if (stopRequested)
        { throw EndGame(winner); }

        cout << endl << conn[0] << " vs " << conn[1] << endl << endl;
        print(cout, fields);

        cout << "Health: ";
        for (auto ship: fields[0].get_ships())
        { cout << ship.get_health() << ' '; }
        cout << endl;

        if (i%2)
        {
            for (;;)
            {
                cout << "Guess where (x, y): ";
                cin >> x >> y;
                cout << endl;
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
            if (fields[0].attack(hehe))
            {
                ++i;
                fields[0].get_ship(hehe).damage(1);
            }
            conn.result(hehe, fields[0].attack(hehe), fields[0].is_alive());

            if (!fields[0].is_alive())
            { throw EndGame(conn[1]); }
        }
    }
    return 1;
}

int Warship::game()
{
    GameWindow *window = new GameWindow;
    window->show();

    QEventLoop loop;
    int result = 0;

    // Игрок проиграл
    QObject::connect(window, &GameWindow::losePressed,
                     &loop, [window, &result, this, &loop]() {
        emit closeGame();
        window->close();
        winner = conn[1];
        result = 1;
        stopRequested = true;
        loop.quit();
    });

    // Закрытие окна → стоп логики
    QObject::connect(window, &QObject::destroyed,
                     [&](){
        stopRequested = true;
    });

    // ────────────── Запуск потока ──────────────
    std::promise<void> prom;
    std::future<void> fut = prom.get_future();

    std::thread([this, &prom, &loop]() {
        try {
            this->logic();
            QMetaObject::invokeMethod(&loop, "quit", Qt::QueuedConnection);
            prom.set_value();
        }
        catch (...) {
            try { prom.set_exception(std::current_exception()); } catch(...) {}
            QMetaObject::invokeMethod(&loop, "quit", Qt::QueuedConnection);
        }
    }).detach();

    loop.exec(); // ждём любой из трёх событий

    window->close();
    window->deleteLater();

    // If the worker thread signalled an exception, rethrow it so the
    // caller (main) can handle it in its try/catch.
    fut.get();

    stopRequested = true; // страховка

    return result;
}

string Warship::get_invite()
{ return (conn.get_invite()); }