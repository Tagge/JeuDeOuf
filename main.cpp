#include "gamewindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GameWindow w;
    w.show();
    a.setStyleSheet("QPushButton { background-color: red; }");
    return a.exec();
}


