#ifndef MOJE_KLASY_H
#define MOJE_KLASY_H

#include "QPainter"

enum ruch {
    lewo,
    prawo,
    gora,
    dol,
    brak
};

enum rodzaj {
    gracz_enum,
    duch_enum
};

class postac {
public:
    int x;
    int y;
    int x_pole;
    int y_pole;
    int x_pole_poczatkowe;
    int y_pole_poczatkowe;
    rodzaj typ;
    ruch kierunek_ruchu;
    ruch temp;
    int liczba_wywolan_mozliwych_na_zmiane_ruchu;
    QPixmap *postac_obrazek;
    ~postac();
};

class gracz : public postac {
public:
    int punkty_kulki;
    int punkty_duchy;
    int zycia;
    //konstruktor:
    gracz(int x_pole, int y_pole, rodzaj typ);
};

class duch : public postac {
public:
    QPixmap *postac_mozliwosc_zjadania_obrazek;
    bool mozliwosc_zjadania_przez_gracza;
    bool aktywny;
    bool zjedzony;
public:
    //konstruktor:
    duch(int x_pole, int y_pole, rodzaj typ, int numer_ducha);
    ~duch();

};

#endif // MOJE_KLASY_H
