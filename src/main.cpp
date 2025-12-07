// #include <QApplication>
// #include <QDebug>
// #include "gui/window.h"
// #include "point.h"
// #include "alg.cpp"

// int main(int argc, char *argv[]) {
//     QApplication app(argc, argv);

//     Window w;
//     w.show();

//     QObject::connect(&w, &Window::cellClicked, [&w](const QString& coord){
//         qDebug() << "from main:" << coord;
//         w.setMiddleText(std::string("Player1"));
//         w.setCellColor(Point(coord.toStdString()), 1);
//         w.setCellColor(Point(coord.toStdString()), 0);
//     });

//     QObject::connect(&w, &Window::losePressed, [](){
//         qDebug() << "Player lost!";
//     });

//     return app.exec();
// }


#include <iostream>
#include "warship.h"
using namespace std;

int main(int argc, char** argv)
{
    string name;
    cout << "Enter your name: ";
    getline(cin, name);

    string host_;
    cout << "Enter your invite code: ";
    getline(cin, host_);
    host_ = (host_.empty()) ? "127.0.0.1" : host_;
    cout << get_invite(host_) << endl;

    unsigned port = (argc-1) ? (unsigned)stoul(argv[1]) : 5555;
    cout << "Port is " << port << endl;
    Warship nemat{host_, port, name}; 

    try
    { nemat.game(); }
    catch (const EndGame& e) 
    { std::cerr << e.what() << " WIN!!" << '\n'; }
}