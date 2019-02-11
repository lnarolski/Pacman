#ifndef PACMAN_H
#define PACMAN_H

#include <QMainWindow>
#include "gra.h"

namespace Ui {
class Pacman;
}

class Pacman : public QMainWindow
{
    Q_OBJECT

public:
    explicit Pacman(QWidget *parent = 0);
    ~Pacman();

private slots:
    void on_graj_clicked();

private:
    Ui::Pacman *ui;
    Gra *okno_gry;
};

#endif // PACMAN_H
