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

    // Register Point as a metatype so it can be used in queued signal/slot
    qRegisterMetaType<Point>("Point");

    // Window w;
    // w.show();

    // QObject::connect(&w, &Window::cellClicked, [&w](const QString& coord){
    //     qDebug() << "from main:" << coord;
    //     w.setMiddleText(std::string("Player1"));
    //     w.setCellColor(Point(coord.toStdString()), 1);
    //     w.setCellColor(Point(coord.toStdString()), 0);
    // });

    // QObject::connect(&w, &Window::losePressed, [](){
    //     qDebug() << "Player lost!";
    // });

    std::string name;
    std::cout << "Enter your name: ";
    std::getline(std::cin, name);

    std::string host_;
    std::cout << "Enter your invite code: ";
    std::getline(std::cin, host_);
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

    // Show a final window announcing the winner (if any) and run the
    // Qt event loop so the window is visible until the user closes it.
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


// #include <iostream>
// #include "warship.h"
// using namespace std;

// int main(int argc, char** argv)
// {
//     string name;
//     cout << "Enter your name: ";
//     getline(cin, name);

//     string host_;
//     cout << "Enter your invite code: ";
//     getline(cin, host_);
//     host_ = (host_.empty()) ? "127.0.0.1" : host_;
//     cout << host_ << endl;

//     unsigned port = (argc-1) ? (unsigned)stoul(argv[1]) : 5555;
//     cout << "Port is " << port << endl;
//     Warship nemat{host_, port, name}; 

//     try
//     { nemat.game(); }
//     catch (const EndGame& e) 
//     { std::cerr << e.what() << " WIN!!" << '\n'; }
// }