#ifndef GRA_H
#define GRA_H

#include <QWidget>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <iostream>
#include <QTableWidget>
#include "moje_klasy.h"

namespace Ui {
class Gra;
}

class Gra : public QWidget
{
    Q_OBJECT

public:
    explicit Gra(QString nazwa_mapy_konstruktor, QWidget *parent = 0);
    ~Gra();
    void set_nazwa_mapy(QString nazwa);
    QString get_nazwa_mapy();
    enum pola_mapy {
        sciana,
        nie_istnieje,
        punkt,
        zjadanie_duchow,
        respawn_duchow,
        wyjscie_duchow,
        puste_pole,
        pole_poczatkowe,
        tunel,
        skrzyzowanie,
        skrzyzowanie_punkt,
        skrzyzowanie_duchy
    };

//sloty wykorzystywane w timerach
private slots:
    void dzialanie_gry();
    void aktywacja();
    void mozliwosc_zjadania_duchow();
    void aktywacja_duch0();
    void aktywacja_duch1();
    void aktywacja_duch2();

private:
    Ui::Gra *ui;
    QString nazwa_mapy;
    QFile *plik_z_mapa;
    QTextStream *in;
    int szerokosc;
    int wysokosc;
    int punkty_do_zdobycia;
    pola_mapy **mapa;
    QTimer *timer;
    QTimer *timer_aktywacja;
    QTimer *timer_aktywacja_duch0;
    QTimer *timer_aktywacja_duch1;
    QTimer *timer_aktywacja_duch2;
    QTimer *timer_mozliwosc_zjadania_duchow;
    gracz *gracz_obiekt;
    duch *duch_obiekt[3];
    int numer_ducha_do_aktywacji;
    void rysowanie_postaci(QPixmap *mapa_obrazek);
    void ruch_gracz();
    void ruch_duchy();
    void interakcje();
    void keyPressEvent(QKeyEvent *event);
};

#endif // GRA_H
