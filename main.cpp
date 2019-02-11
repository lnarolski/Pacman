#include "pacman.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Pacman w;
    w.show();

    return a.exec();
}
