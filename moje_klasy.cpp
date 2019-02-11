#include "moje_klasy.h"

//klasa gracza
gracz::gracz(int x_pole, int y_pole, rodzaj typ) {
    punkty_kulki = 0;
    punkty_duchy = 0;
    zycia = 3;
    this->x_pole = x_pole;
    this->y_pole = y_pole;
    this->typ = typ;
    x_pole_poczatkowe = x_pole;
    y_pole_poczatkowe = y_pole;
    x = x_pole*40;
    y = y_pole*40;
    kierunek_ruchu = brak;

    postac_obrazek = new QPixmap(40, 40);
    postac_obrazek->fill(QColor(0,0,0,0)); //ustawienie przezroczystych postaci
    QPainter painter(postac_obrazek);
    painter.setBrush(Qt::green);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(*new QPoint(20, 20), 15, 15);
    painter.end();
}

//klasa duchow
duch::duch(int x_pole, int y_pole, rodzaj typ, int numer_ducha) {
    this->x_pole = x_pole;
    this->y_pole = y_pole;
    x_pole_poczatkowe = x_pole;
    y_pole_poczatkowe = y_pole;
    this->typ = typ;
    x = x_pole*40;
    y = y_pole*40;
    kierunek_ruchu = brak;

    postac_obrazek = new QPixmap(40, 40);
    postac_obrazek->fill(QColor(0,0,0,0)); //ustawienie przezroczystych postaci
    QPainter painter(postac_obrazek);
    switch (numer_ducha) {
    case 0:
        painter.setBrush(Qt::red);
        break;
    case 1:
        painter.setBrush(Qt::cyan);
        break;
    default:
        painter.setBrush(Qt::gray);
        break;
    }
    painter.setPen(Qt::NoPen);
    static const QPointF points[3] = {
        QPointF(20.0, 0.0),
        QPointF(40.0, 40.0),
        QPointF(0.0, 40.0)
    };
    painter.drawConvexPolygon(points, 3);
    painter.end();

    postac_mozliwosc_zjadania_obrazek = new QPixmap(40, 40);
    postac_mozliwosc_zjadania_obrazek->fill(QColor(0,0,0,0)); //ustawienie przezroczystych postaci
    QPainter painter2(postac_mozliwosc_zjadania_obrazek);
    painter2.setBrush(Qt::NoBrush);
    switch (numer_ducha) {
    case 0:
        painter2.setPen(Qt::red);
        break;
    case 1:
        painter2.setPen(Qt::cyan);
        break;
    default:
        painter2.setPen(Qt::gray);
        break;
    }
    painter2.drawConvexPolygon(points, 3);
    painter2.end();

    mozliwosc_zjadania_przez_gracza = false;
    aktywny = false;
    zjedzony = false;
}

duch::~duch() {
    delete postac_mozliwosc_zjadania_obrazek;
    //delete timer_aktywacja;
}

postac::~postac() {
    delete postac_obrazek;
}
