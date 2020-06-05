#include "gamewindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GameWindow w;
    w.show();
    a.setStyleSheet("QPushButton { "
                    "background-color: #CC4D4D;"
                    "border: 1px solid #26292B;"
                    "color: white; "
                    "font: serif;}"
                    "QPushButton:hover {"
                    "background-color: #EF8A8A;}");
    return a.exec();
}


