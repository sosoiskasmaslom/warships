#include <QApplication>
#include <QDebug>
#include <iostream>
#include "warship.h"
#include "gui/window.h"
#include "point.h"
#include "alg.cpp"
#include <QMetaType>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    qRegisterMetaType<Point>("Point");

    QString qname;
    QString qhost;
    {
        NameInviteWindow prompt;
        QEventLoop loop;
        QObject::connect(&prompt, &NameInviteWindow::finished, 
            [&](const QString& n, const QString& h)
        {
            qname = n;
            qhost = h;
            loop.quit();
        });
        prompt.show();
        loop.exec();
    }

    std::string name = qname.toStdString();
    std::string host_ = qhost.toStdString();
    host_ = (host_.empty()) ? "127.0.0.1" : host_;

    unsigned port = (argc-1) ? (unsigned)std::stoul(argv[1]) : 5555;
    std::cout << "Port is " << port << std::endl;
    Warship nemat{host_, port, name}; 

    std::string winnerName;
    try {
        nemat.game();
    }
    catch (const EndGame& e) {
        winnerName = e.what();
    }

    WinWindow *win = new WinWindow;
    if (!winnerName.empty()) {
        QString msg = QString::fromStdString(winnerName) + " WIN!!";
        win->setMessage(msg);
    } else {
        win->setMessage("Game finished");
    }
    win->show();

    return app.exec();
}
