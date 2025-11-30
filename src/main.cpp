#include <QApplication>
#include <QDebug>
#include "gui/window.h"
#include "point.h"
#include "alg.cpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Window w;
    w.show();

    QObject::connect(&w, &Window::cellClicked, [&w](const QString& coord){
        qDebug() << "from main:" << coord;
        w.setMiddleText(std::string("Player1"));
        w.setCellColor(Point(coord.toStdString()), 1);
        w.setCellColor(Point(coord.toStdString()), 0);
    });

    return app.exec();
}
