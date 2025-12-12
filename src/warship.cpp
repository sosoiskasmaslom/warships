
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

    label_line = conn[0] + string(" vs ") + conn[1];
    filling_ships();
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

    for (unsigned heh, x, y, i = conn.get_host(); ; ++i)
    {
        if (stopRequested)
            throw EndGame(winner);

        if (i % 2)
        {
            QMetaObject::invokeMethod(window, "setRightClickable",
                                      Qt::QueuedConnection,
                                      Q_ARG(bool, true));

            {
                std::lock_guard<std::mutex> lk(click_m);
                clicked.reset();
            }

            Point chosen;
            {
                std::unique_lock<std::mutex> lk(click_m);
                click_cv.wait(lk, [this]{ return clicked.has_value() || stopRequested.load(); });
                if (stopRequested)
                    throw EndGame(winner);
                chosen = clicked->trans();
                clicked.reset();
            }

            QMetaObject::invokeMethod(window, "setRightClickable",
                                      Qt::QueuedConnection,
                                      Q_ARG(bool, false));

            conn.shot(chosen);
            unsigned s = conn.result();
            i += (s>0);

            fields[1].add_try(Point(conn.get_shot()), (s>0));
            emit cellColor(Point(conn.get_shot()), s);
        }
        else
        {
            Point hehe = Point(conn.shot());
            unsigned hit = fields[0].attack(hehe);

            if (hit) {
                ++i;
                fields[0].get_ship(hehe).damage(1);
            }

            emit an_cellColor(hehe, hit);
            conn.result(hehe, hit, fields[0].is_alive());

            if (!fields[0].is_alive())
                throw EndGame(conn[1]);
        }
    }

    return true;
}

int Warship::game()
{
    GameWindow *window = new GameWindow;
    this->window = window;
    window->show();
    window->setMiddleText(label_line);

    QEventLoop loop;
    int result = 0;

    for (auto ship: fields[0].get_ships())
    {
        for(int i = 0; i<ship.get_length(); ++i)
        {
            if (ship.get_vertical()) {
                window->setCellColor(Point(ship.get_coord()[1]+i, ship.get_coord()[0]), 0, QString("green"));
            } else {
                window->setCellColor(Point(ship.get_coord()[1], ship.get_coord()[0]+i), 0, QString("green"));
            }
        }
    }

    QObject::connect(window, &GameWindow::losePressed,
                     &loop, [window, &result, this, &loop]() {
        conn.bue();
        window->close();
        winner = conn[1];

        result = 1;
        stopRequested = true;
        loop.quit();
    });

    QObject::connect(this, &Warship::cellColor, [&](Point coord, unsigned s){
        QString color = (s) ? QString("green") : QString("red");
        if (s>1) 
        { color = QString("blue"); }
        window->setCellColor(coord.trans(), 1, color);
    });

    QObject::connect(this, &Warship::an_cellColor, [&](Point coord, bool s){
        QString color = (s) ? QString("red") : QString("grey");
        window->setCellColor(coord.trans(), 0, color);
    });

    QObject::connect(window, &GameWindow::cellClicked, this, &Warship::onCellClicked);

    QObject::connect(window, &QObject::destroyed, [&](){
        stopRequested = true;
    });

    window->setRightClickable(false);

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

    loop.exec();

    window->close();
    window->deleteLater();

    fut.get();

    stopRequested = true;

    return result;
}

void Warship::filling_ships()
{
    ship_window = new DragDropWindow;
    ship_window->show();

    QEventLoop loop;
    QObject::connect(ship_window, &DragDropWindow::shipsPlaced, &loop, &QEventLoop::quit);
    QObject::connect(ship_window, &QObject::destroyed, &loop, &QEventLoop::quit);
    loop.exec();

    if (ship_window) {
        auto placed = ship_window->getPlacedShips();
        for (auto &t : placed) {
            if (t.size() >= 4) {
                fields[0].add_ship(t[0], t[1], t[2], (t[3] != 0));
            }
        }
            ship_window->deleteLater();
            ship_window = nullptr;
    }
}

string Warship::get_invite()
{ return (conn.get_invite()); }

void Warship::onCellClicked(const Point& p)
{
    std::lock_guard<std::mutex> lk(click_m);
    clicked = p;
    click_cv.notify_one();
}