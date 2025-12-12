
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
            bool s = conn.result();
            i += s;

            fields[1].add_try(Point(conn.get_shot()), s);
            emit cellColor(Point(conn.get_shot()), s);
        }
        else
        {
            Point hehe = Point(conn.shot());
            bool hit = fields[0].attack(hehe);

            if (hit) {
                ++i;
                fields[0].get_ship(hehe).damage(1);
            }

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
    this->window = window; // expose to logic()
    window->show();

    QEventLoop loop;
    int result = 0;

    // Игрок проиграл
    QObject::connect(window, &GameWindow::losePressed,
                        &loop, [window, &result, this, &loop]() {
        // Send BUE to opponent immediately so their side will receive
        // EndGame and quit. Close our window and signal logic to stop.
        conn.bue();
        window->close();
        winner = conn[1];

        result = 1;
        stopRequested = true;
        loop.quit();
    });

    // Connect our cell update signal to the window slot (queued for thread-safety)
    QObject::connect(this, &Warship::cellColor, [&](Point coord, bool s){
        QString color = (s) ? QString("green") : QString("red");
        window->setCellColor(coord.trans(), 1, color);
    });

    // Forward GUI cell clicks to this object's slot which will notify the
    // logic thread (via condition_variable).
    QObject::connect(window, &GameWindow::cellClicked, this, &Warship::onCellClicked);

    // Закрытие окна → стоп логики
    QObject::connect(window, &QObject::destroyed,
                     [&](){
        stopRequested = true;
    });

    // ensure right field is disabled initially (until player turn)
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

void Warship::onCellClicked(const Point& p)
{
    std::lock_guard<std::mutex> lk(click_m);
    clicked = p;
    click_cv.notify_one();
}