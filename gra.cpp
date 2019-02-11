#define SZYBKOSC_RUCHU 4 //liczba musi być dzielnikiem liczby 40 i podzielna na 2
#define ODLEGLOSC_ZJADANIA 10
#define CZAS_NA_ZJADANIE 12000
#define LICZBA_WYWOLAN 15

#include "gra.h"
#include "ui_gra.h"
#include "QFile"
#include "QTableWidget"
#include "QScrollBar"
#include "QTimer"
#include "QPainter"
#include "moje_klasy.h"
#include "QKeyEvent"
#include <windows.h>
#include <math.h>

//konstruktor okna z gra - przeprowadza konfiguracje(wczytuje mape, ustawia timery, label'y, itp.)
Gra::Gra(QString nazwa_mapy_konstruktor, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Gra)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    set_nazwa_mapy(nazwa_mapy_konstruktor);
    plik_z_mapa = new QFile("maps/" + get_nazwa_mapy());
    plik_z_mapa->open(QIODevice::ReadOnly | QIODevice::Text);
    in = new QTextStream(plik_z_mapa);
    QString parametry_mapy = in->readLine();
    QString szerokosc_QString = parametry_mapy;
    QString wysokosc_QString = parametry_mapy;
    szerokosc_QString.truncate(parametry_mapy.indexOf(","));
    wysokosc_QString.remove(0, parametry_mapy.indexOf(",") + 1);
    szerokosc = szerokosc_QString.toInt();
    wysokosc = wysokosc_QString.toInt();
    this->resize(szerokosc*40+200, wysokosc*40);
    int liczba_duchow = 0;
    numer_ducha_do_aktywacji = 0;
    punkty_do_zdobycia = 0;
    timer_mozliwosc_zjadania_duchow = NULL;

    mapa = new pola_mapy *[wysokosc]; //alokacja pamieci
    //wczytanie mapy z pliku do tablicy "mapa"
    for ( int i = 0; i < wysokosc; i++ )
    {
        mapa[i] = new pola_mapy [szerokosc]; //alokacja pamieci
        for ( int j = 0; j < szerokosc; j++) { //wpisanie wartosci do tablicy
            switch (in->readLine().toInt()) {
                case 0:
                    mapa[i][j] = sciana;
                    break;
                case -1:
                    mapa[i][j] = nie_istnieje;
                    break;
                case 1:
                    mapa[i][j] = punkt;
                    punkty_do_zdobycia += 10;
                    break;
                case 2:
                    mapa[i][j] = zjadanie_duchow;
                    break;
                case 5:
                    mapa[i][j] = respawn_duchow;
                    duch_obiekt[liczba_duchow] = new duch(j,i,duch_enum,liczba_duchow);
                    liczba_duchow++;
                    break;
                case 7:
                    mapa[i][j] = wyjscie_duchow;
                    break;
                case 8:
                    mapa[i][j] = puste_pole;
                    break;
                case 9:
                    mapa[i][j] = pole_poczatkowe;
                    gracz_obiekt = new gracz(j,i,gracz_enum);
                    break;
                case 10:
                    mapa[i][j] = skrzyzowanie;
                    break;
                case 11:
                    mapa[i][j] = skrzyzowanie_punkt;
                    punkty_do_zdobycia += 10;
                    break;
                case 12:
                    mapa[i][j] = skrzyzowanie_duchy;
                    duch_obiekt[liczba_duchow] = new duch(j,i,duch_enum,liczba_duchow);
                    liczba_duchow++;
                    break;
                case 18:
                    mapa[i][j] = tunel;
                    break;
                default:
                    break;
            }
        }
    }
    // i = wiersz; j = kolumna
    //konfiguracja uzywanych label'i
    ui->imageLabel->resize(szerokosc*40, wysokosc*40);
    ui->zycia->move(szerokosc*40, wysokosc*40/10);
    ui->punkty->move(szerokosc*40, wysokosc*40/6);
    ui->czas_zjadania->move(szerokosc*40, wysokosc*40/4);
    ui->zycia->resize(200, 30);
    ui->punkty->resize(200, 30);
    ui->czas_zjadania->resize(200, 30);
    ui->zycia->setAlignment(Qt::AlignCenter);
    ui->punkty->setAlignment(Qt::AlignCenter);
    ui->czas_zjadania->setAlignment(Qt::AlignCenter);

    //uruchomienie timer'ow
    timer = new QTimer(this); //podstawowy timer obslugujacy dzialanie gry
    connect(timer, SIGNAL(timeout()), this, SLOT(dzialanie_gry()));
    timer->start(50);

    timer_aktywacja = new QTimer(this); //timer aktywujacy duchy co 3 s
    connect(timer_aktywacja, SIGNAL(timeout()), this, SLOT(aktywacja()));
    timer_aktywacja->start(3000);

    this->show(); //pokazanie okna z plansza po konfiguracji
}

//dekonstruktor okna z gra
Gra::~Gra()
{
    delete ui;
    plik_z_mapa -> close();
    delete plik_z_mapa;
    delete in;
    delete timer;
    delete timer_aktywacja;

    for (int i = 0; i < wysokosc; i++)
        delete [] mapa[i]; //uwolnienie pamieci
    delete [] mapa; //uwolnienie pamieci

    delete gracz_obiekt;
    delete duch_obiekt[0];
    delete duch_obiekt[1];
    delete duch_obiekt[2];
}

//metoda ustawiajaca nazwe pliku z mapa
void Gra::set_nazwa_mapy(QString nazwa) {
    nazwa_mapy = nazwa;
}

//metoda zwracajaca nazwe pliku z mapa
QString Gra::get_nazwa_mapy() {
    return nazwa_mapy;
}

//funkcja wykonujaca sie zgodnie z timerem "timer" co 50 ms - odpowiada za dzialanie calej gry
void Gra::dzialanie_gry() {
    ruch_gracz();
    ruch_duchy();
    interakcje();
    QPixmap *mapa_obrazek = new QPixmap(szerokosc*40, wysokosc*40);
    QPainter painter(mapa_obrazek);
    QPoint *punkt_temp;
    //rysowanie planszy na ekranie
    for ( int i = 0; i < wysokosc; i++ )
    {
        for ( int j = 0; j < szerokosc; j++) {
            switch (mapa[i][j]) {
                case sciana:
                    painter.setBrush(Qt::blue);
                    painter.setPen(Qt::NoPen);
                    painter.drawRect(j * 40, i * 40, 40, 40);
                    break;
                case nie_istnieje:

                    break;
                case punkt:
                    painter.setBrush(Qt::yellow);
                    painter.setPen(Qt::NoPen);
                    punkt_temp = new QPoint(j * 40 + 20, i * 40 + 20);
                    painter.drawEllipse(*punkt_temp, 5, 5);
                    delete punkt_temp;
                    break;
                case skrzyzowanie_punkt:
                    painter.setBrush(Qt::yellow);
                    painter.setPen(Qt::NoPen);
                    punkt_temp = new QPoint(j * 40 + 20, i * 40 + 20);
                    painter.drawEllipse(*punkt_temp, 5, 5);
                    delete punkt_temp;
                    break;
                case zjadanie_duchow:
                    painter.setBrush(Qt::yellow);
                    painter.setPen(Qt::NoPen);
                    punkt_temp = new QPoint(j * 40 + 20, i * 40 + 20);
                    painter.drawEllipse(*punkt_temp, 10, 10);
                    delete punkt_temp;
                    break;
                case respawn_duchow:

                    break;
                case wyjscie_duchow:
                    painter.setBrush(Qt::yellow);
                    painter.setPen(Qt::NoPen);
                    painter.drawRect(j * 40, i * 40 + 10, 40, 20);
                    break;
                case puste_pole:
                    painter.setBrush(Qt::black);
                    painter.setPen(Qt::NoPen);
                    painter.drawRect(j * 40, i * 40, 40, 40);
                    break;
                case skrzyzowanie_duchy:
                    painter.setBrush(Qt::black);
                    painter.setPen(Qt::NoPen);
                    painter.drawRect(j * 40, i * 40, 40, 40);
                    break;
                case skrzyzowanie:
                    painter.setBrush(Qt::black);
                    painter.setPen(Qt::NoPen);
                    painter.drawRect(j * 40, i * 40, 40, 40);
                    break;
                case tunel:
                    painter.setBrush(Qt::black);
                    painter.setPen(Qt::NoPen);
                    painter.drawRect(j * 40, i * 40, 40, 40);
                    break;
                case pole_poczatkowe:

                    break;
            }
        }
    }
    painter.end();
    rysowanie_postaci(mapa_obrazek); //rysowanie postaci na narysowanej wczesniej planszy
    ui->imageLabel->setPixmap(*mapa_obrazek);
    ui->punkty->setText("Punkty:\n" + QString::number(gracz_obiekt->punkty_kulki + gracz_obiekt->punkty_duchy));
    ui->zycia->setText("Zycia:\n" + QString::number(gracz_obiekt->zycia));
    if (timer_mozliwosc_zjadania_duchow != NULL) {
        ui->czas_zjadania->setText("Czas zjadania:\n" + QString::number(timer_mozliwosc_zjadania_duchow->remainingTime()/1000) + " s");
    }
    else {
        ui->czas_zjadania->setText("");
    }
    delete mapa_obrazek;
}

//metoda odpowiedzialna za rysowanie postaci znajdujacych sie w grze na planszy
void Gra::rysowanie_postaci(QPixmap *mapa_obrazek) {
    QPainter painter(mapa_obrazek);
    QPoint *punkt = new QPoint(gracz_obiekt->x,gracz_obiekt->y);
    painter.drawPixmap(*punkt,*gracz_obiekt->postac_obrazek);
    for (int i = 0; i < 3; i++) {
        QPoint *punkt = new QPoint(duch_obiekt[i]->x,duch_obiekt[i]->y);
        if(duch_obiekt[i]->mozliwosc_zjadania_przez_gracza)
            painter.drawPixmap(*punkt,*duch_obiekt[i]->postac_mozliwosc_zjadania_obrazek);
        else
            painter.drawPixmap(*punkt,*duch_obiekt[i]->postac_obrazek);
        delete punkt;
    }
    painter.end();
    delete punkt;
}

//metoda odpwoedzialna za ruchy i decyzje podejmowane przez duchy
void Gra::ruch_duchy() {
    for (int i = 0; i < 3; i++) {
        //sprawdzenie czy mozna juz aktywowac zjedzonego ducha po zakonczeniu czasu na ich zjadanie przez gracza
        if (!duch_obiekt[i] -> zjedzony && timer_mozliwosc_zjadania_duchow == NULL && !timer_aktywacja -> isActive()) {
            duch_obiekt[i] -> aktywny = true;
        }
        //ruch aktywnego ducha
        //DZIWNE WARUNKI I WSPOLRZEDNE, BO FUNKCJE OD RUCHU BADALY STAN PLANSZY POZA TABLICAMI!!!
        if (duch_obiekt[i] -> aktywny) {
            //teleportacja duchow po przejsciu tunelu
            if (duch_obiekt[i] -> x < 0 + 40 - 40 / SZYBKOSC_RUCHU) { //teleportacja na prawo
                duch_obiekt[i] -> x = (szerokosc - 2) * 40;
                duch_obiekt[i] -> x_pole = szerokosc - 2;
            }
            else if (duch_obiekt[i] -> x >= szerokosc * 40 - 40 - 40 / SZYBKOSC_RUCHU) { //teleportacja na lewo
                duch_obiekt[i] -> x = 40;
                duch_obiekt[i] -> x_pole = 1;
            }
            else if (duch_obiekt[i] -> y < 0 + 40 - 40 / SZYBKOSC_RUCHU) { //teleportacja na dol
                duch_obiekt[i] -> y = (wysokosc - 2) * 40;
                duch_obiekt[i] -> y_pole = wysokosc - 2;
            }
            else if (duch_obiekt[i] -> y >= wysokosc * 40 - 40 - 40 / SZYBKOSC_RUCHU) { //teleportacja na gore
                duch_obiekt[i] -> y = 40;
                duch_obiekt[i] -> y_pole = 1;
            }
            //decyzja o kierunku ruchu ducha, gdy znajduje sie on na calym polu
            if ((duch_obiekt[i] ->x % 40 == 0) && (duch_obiekt[i] ->y % 40 == 0)) {
                duch_obiekt[i] ->x_pole = duch_obiekt[i] ->x / 40;
                duch_obiekt[i] ->y_pole = duch_obiekt[i] ->y / 40;
                if(duch_obiekt[i] -> mozliwosc_zjadania_przez_gracza) {
                    if (mapa[duch_obiekt[i] -> y_pole][duch_obiekt[i] -> x_pole] == respawn_duchow || mapa[duch_obiekt[i] -> y_pole][duch_obiekt[i] -> x_pole] == skrzyzowanie_duchy) {
                        if (mapa[duch_obiekt[i] -> y_pole][duch_obiekt[i] -> x_pole + 1] == skrzyzowanie_duchy)
                            duch_obiekt[i] -> kierunek_ruchu = prawo;
                        else if (mapa[duch_obiekt[i] -> y_pole][duch_obiekt[i] -> x_pole - 1] == skrzyzowanie_duchy)
                            duch_obiekt[i] -> kierunek_ruchu = lewo;
                        else
                            duch_obiekt[i] -> kierunek_ruchu = gora;
                    }
                    else {
                        float odleglosc = std::numeric_limits<float>::min(); //maksymalna wartość dla float
                        if (mapa[duch_obiekt[i] -> y_pole][duch_obiekt[i] -> x_pole] == skrzyzowanie || mapa[duch_obiekt[i] -> y_pole][duch_obiekt[i] -> x_pole] == skrzyzowanie_punkt) {
                            if (mapa[duch_obiekt[i] -> y_pole - 1][duch_obiekt[i] -> x_pole] != sciana) {
                                if (sqrt(pow(duch_obiekt[i] -> x_pole - gracz_obiekt -> x_pole,2) + pow(duch_obiekt[i] -> y_pole - 1 - gracz_obiekt -> y_pole,2)) > odleglosc) {
                                    odleglosc = sqrt(pow(duch_obiekt[i] -> x_pole - gracz_obiekt -> x_pole,2) + pow(duch_obiekt[i] -> y_pole - 1 - gracz_obiekt -> y_pole,2));
                                    duch_obiekt[i] -> kierunek_ruchu = gora;
                                }

                            }
                            if ((mapa[duch_obiekt[i] -> y_pole + 1][duch_obiekt[i] -> x_pole] != sciana) && (mapa[duch_obiekt[i] -> y_pole + 1][duch_obiekt[i] -> x_pole] != wyjscie_duchow)) {
                                if (sqrt(pow(duch_obiekt[i] -> x_pole - gracz_obiekt -> x_pole,2) + pow(duch_obiekt[i] -> y_pole + 1 - gracz_obiekt -> y_pole,2)) > odleglosc) {
                                    odleglosc = sqrt(pow(duch_obiekt[i] -> x_pole - gracz_obiekt -> x_pole,2) + pow(duch_obiekt[i] -> y_pole + 1 - gracz_obiekt -> y_pole,2));
                                    duch_obiekt[i] -> kierunek_ruchu = dol;
                                }
                            }
                            if (mapa[duch_obiekt[i] -> y_pole][duch_obiekt[i] -> x_pole + 1] != sciana) {
                                if (sqrt(pow(duch_obiekt[i] -> x_pole + 1 - gracz_obiekt -> x_pole,2) + pow(duch_obiekt[i] -> y_pole - gracz_obiekt -> y_pole,2)) > odleglosc) {
                                    odleglosc = sqrt(pow(duch_obiekt[i] -> x_pole + 1 - gracz_obiekt -> x_pole,2) + pow(duch_obiekt[i] -> y_pole - gracz_obiekt -> y_pole,2));
                                    duch_obiekt[i] -> kierunek_ruchu = prawo;
                                }

                            }
                            if (mapa[duch_obiekt[i] -> y_pole][duch_obiekt[i] -> x_pole - 1] != sciana) {
                                if (sqrt(pow(duch_obiekt[i] -> x_pole - 1 - gracz_obiekt -> x_pole,2) + pow(duch_obiekt[i] -> y_pole - gracz_obiekt -> y_pole,2)) > odleglosc) {
                                    odleglosc = sqrt(pow(duch_obiekt[i] -> x_pole - 1 - gracz_obiekt -> x_pole,2) + pow(duch_obiekt[i] -> y_pole - gracz_obiekt -> y_pole,2));
                                    duch_obiekt[i] -> kierunek_ruchu = lewo;
                                }
                            }

                        }
                        else {
                            if (duch_obiekt[i] -> kierunek_ruchu == gora && mapa[duch_obiekt[i] -> y_pole - 1][duch_obiekt[i] -> x_pole] == sciana) {
                                if (mapa[duch_obiekt[i] -> y_pole][duch_obiekt[i] -> x_pole + 1] != sciana)
                                    duch_obiekt[i] -> kierunek_ruchu = prawo;
                                else if (mapa[duch_obiekt[i] -> y_pole][duch_obiekt[i] -> x_pole - 1] != sciana)
                                    duch_obiekt[i] -> kierunek_ruchu = lewo;
                                else
                                    duch_obiekt[i] -> kierunek_ruchu = dol;

                            }
                            else if (duch_obiekt[i] -> kierunek_ruchu == dol && mapa[duch_obiekt[i] -> y_pole + 1][duch_obiekt[i] -> x_pole] == sciana) {
                                if (mapa[duch_obiekt[i] -> y_pole][duch_obiekt[i] -> x_pole + 1] != sciana)
                                    duch_obiekt[i] -> kierunek_ruchu = prawo;
                                else if (mapa[duch_obiekt[i] -> y_pole][duch_obiekt[i] -> x_pole - 1] != sciana)
                                    duch_obiekt[i] -> kierunek_ruchu = lewo;
                                else
                                    duch_obiekt[i] -> kierunek_ruchu = gora;
                            }
                            else if (duch_obiekt[i] -> kierunek_ruchu == prawo && mapa[duch_obiekt[i] -> y_pole][duch_obiekt[i] -> x_pole + 1] == sciana) {
                                if (mapa[duch_obiekt[i] -> y_pole - 1][duch_obiekt[i] -> x_pole] != sciana)
                                    duch_obiekt[i] -> kierunek_ruchu = gora;
                                else if (mapa[duch_obiekt[i] -> y_pole + 1][duch_obiekt[i] -> x_pole] != sciana)
                                    duch_obiekt[i] -> kierunek_ruchu = dol;
                                else
                                    duch_obiekt[i] -> kierunek_ruchu = lewo;
                            }
                            else if (duch_obiekt[i] -> kierunek_ruchu == lewo && mapa[duch_obiekt[i] -> y_pole][duch_obiekt[i] -> x_pole - 1] == sciana) {
                                if (mapa[duch_obiekt[i] -> y_pole - 1][duch_obiekt[i] -> x_pole] != sciana)
                                    duch_obiekt[i] -> kierunek_ruchu = gora;
                                else if (mapa[duch_obiekt[i] -> y_pole + 1][duch_obiekt[i] -> x_pole] != sciana)
                                    duch_obiekt[i] -> kierunek_ruchu = dol;
                                else
                                    duch_obiekt[i] -> kierunek_ruchu = prawo;
                            }
                        }
                    }
                }
                else {
                    if (mapa[duch_obiekt[i] -> y_pole][duch_obiekt[i] -> x_pole] == respawn_duchow || mapa[duch_obiekt[i] -> y_pole][duch_obiekt[i] -> x_pole] == skrzyzowanie_duchy) {
                        if (mapa[duch_obiekt[i] -> y_pole][duch_obiekt[i] -> x_pole + 1] == skrzyzowanie_duchy)
                            duch_obiekt[i] -> kierunek_ruchu = prawo;
                        else if (mapa[duch_obiekt[i] -> y_pole][duch_obiekt[i] -> x_pole - 1] == skrzyzowanie_duchy)
                            duch_obiekt[i] -> kierunek_ruchu = lewo;
                        else
                            duch_obiekt[i] -> kierunek_ruchu = gora;
                    }
                    else {
                        float odleglosc = std::numeric_limits<float>::max(); //maksymalna wartość dla float
                        if (mapa[duch_obiekt[i] -> y_pole][duch_obiekt[i] -> x_pole] == skrzyzowanie || mapa[duch_obiekt[i] -> y_pole][duch_obiekt[i] -> x_pole] == skrzyzowanie_punkt) {
                            if (mapa[duch_obiekt[i] -> y_pole - 1][duch_obiekt[i] -> x_pole] != sciana) {
                                if (sqrt(pow(duch_obiekt[i] -> x_pole - gracz_obiekt -> x_pole,2) + pow(duch_obiekt[i] -> y_pole - 1 - gracz_obiekt -> y_pole,2)) < odleglosc) {
                                    odleglosc = sqrt(pow(duch_obiekt[i] -> x_pole - gracz_obiekt -> x_pole,2) + pow(duch_obiekt[i] -> y_pole - 1 - gracz_obiekt -> y_pole,2));
                                    duch_obiekt[i] -> kierunek_ruchu = gora;
                                }

                            }
                            if ((mapa[duch_obiekt[i] -> y_pole + 1][duch_obiekt[i] -> x_pole] != sciana) && (mapa[duch_obiekt[i] -> y_pole + 1][duch_obiekt[i] -> x_pole] != wyjscie_duchow)) {
                                if (sqrt(pow(duch_obiekt[i] -> x_pole - gracz_obiekt -> x_pole,2) + pow(duch_obiekt[i] -> y_pole + 1 - gracz_obiekt -> y_pole,2)) < odleglosc) {
                                    odleglosc = sqrt(pow(duch_obiekt[i] -> x_pole - gracz_obiekt -> x_pole,2) + pow(duch_obiekt[i] -> y_pole + 1 - gracz_obiekt -> y_pole,2));
                                    duch_obiekt[i] -> kierunek_ruchu = dol;
                                }
                            }
                            if (mapa[duch_obiekt[i] -> y_pole][duch_obiekt[i] -> x_pole + 1] != sciana) {
                                if (sqrt(pow(duch_obiekt[i] -> x_pole + 1 - gracz_obiekt -> x_pole,2) + pow(duch_obiekt[i] -> y_pole - gracz_obiekt -> y_pole,2)) < odleglosc) {
                                    odleglosc = sqrt(pow(duch_obiekt[i] -> x_pole + 1 - gracz_obiekt -> x_pole,2) + pow(duch_obiekt[i] -> y_pole - gracz_obiekt -> y_pole,2));
                                    duch_obiekt[i] -> kierunek_ruchu = prawo;
                                }

                            }
                            if (mapa[duch_obiekt[i] -> y_pole][duch_obiekt[i] -> x_pole - 1] != sciana) {
                                if (sqrt(pow(duch_obiekt[i] -> x_pole - 1 - gracz_obiekt -> x_pole,2) + pow(duch_obiekt[i] -> y_pole - gracz_obiekt -> y_pole,2)) < odleglosc) {
                                    odleglosc = sqrt(pow(duch_obiekt[i] -> x_pole - 1 - gracz_obiekt -> x_pole,2) + pow(duch_obiekt[i] -> y_pole - gracz_obiekt -> y_pole,2));
                                    duch_obiekt[i] -> kierunek_ruchu = lewo;
                                }
                            }

                        }
                        else {
                            if (duch_obiekt[i] -> kierunek_ruchu == gora && mapa[duch_obiekt[i] -> y_pole - 1][duch_obiekt[i] -> x_pole] == sciana) {
                                if (mapa[duch_obiekt[i] -> y_pole][duch_obiekt[i] -> x_pole + 1] != sciana)
                                    duch_obiekt[i] -> kierunek_ruchu = prawo;
                                else if (mapa[duch_obiekt[i] -> y_pole][duch_obiekt[i] -> x_pole - 1] != sciana)
                                    duch_obiekt[i] -> kierunek_ruchu = lewo;
                                else
                                    duch_obiekt[i] -> kierunek_ruchu = dol;

                            }
                            else if (duch_obiekt[i] -> kierunek_ruchu == dol && mapa[duch_obiekt[i] -> y_pole + 1][duch_obiekt[i] -> x_pole] == sciana) {
                                if (mapa[duch_obiekt[i] -> y_pole][duch_obiekt[i] -> x_pole + 1] != sciana)
                                    duch_obiekt[i] -> kierunek_ruchu = prawo;
                                else if (mapa[duch_obiekt[i] -> y_pole][duch_obiekt[i] -> x_pole - 1] != sciana)
                                    duch_obiekt[i] -> kierunek_ruchu = lewo;
                                else
                                    duch_obiekt[i] -> kierunek_ruchu = gora;
                            }
                            else if (duch_obiekt[i] -> kierunek_ruchu == prawo && mapa[duch_obiekt[i] -> y_pole][duch_obiekt[i] -> x_pole + 1] == sciana) {
                                if (mapa[duch_obiekt[i] -> y_pole - 1][duch_obiekt[i] -> x_pole] != sciana)
                                    duch_obiekt[i] -> kierunek_ruchu = gora;
                                else if (mapa[duch_obiekt[i] -> y_pole + 1][duch_obiekt[i] -> x_pole] != sciana)
                                    duch_obiekt[i] -> kierunek_ruchu = dol;
                                else
                                    duch_obiekt[i] -> kierunek_ruchu = lewo;
                            }
                            else if (duch_obiekt[i] -> kierunek_ruchu == lewo && mapa[duch_obiekt[i] -> y_pole][duch_obiekt[i] -> x_pole - 1] == sciana) {
                                if (mapa[duch_obiekt[i] -> y_pole - 1][duch_obiekt[i] -> x_pole] != sciana)
                                    duch_obiekt[i] -> kierunek_ruchu = gora;
                                else if (mapa[duch_obiekt[i] -> y_pole + 1][duch_obiekt[i] -> x_pole] != sciana)
                                    duch_obiekt[i] -> kierunek_ruchu = dol;
                                else
                                    duch_obiekt[i] -> kierunek_ruchu = prawo;
                            }
                        }
                }
            }
            }
        //zmiana wspolrzednych ducha zgodnie z wybranym kierunkiem
        switch (duch_obiekt[i] -> kierunek_ruchu) {
        case prawo:
            duch_obiekt[i]->x += ((mapa[duch_obiekt[i] -> y_pole][duch_obiekt[i] -> x_pole] == tunel || duch_obiekt[i]->mozliwosc_zjadania_przez_gracza )?SZYBKOSC_RUCHU/2:SZYBKOSC_RUCHU);
            break;
        case lewo:
            duch_obiekt[i]->x -= ((mapa[duch_obiekt[i] -> y_pole][duch_obiekt[i] -> x_pole] == tunel || duch_obiekt[i]->mozliwosc_zjadania_przez_gracza )?SZYBKOSC_RUCHU/2:SZYBKOSC_RUCHU);
            break;
        case gora:
            duch_obiekt[i]->y -= ((mapa[duch_obiekt[i] -> y_pole][duch_obiekt[i] -> x_pole] == tunel || duch_obiekt[i]->mozliwosc_zjadania_przez_gracza )?SZYBKOSC_RUCHU/2:SZYBKOSC_RUCHU);
            break;
        case dol:
            duch_obiekt[i]->y += ((mapa[duch_obiekt[i] -> y_pole][duch_obiekt[i] -> x_pole] == tunel || duch_obiekt[i]->mozliwosc_zjadania_przez_gracza )?SZYBKOSC_RUCHU/2:SZYBKOSC_RUCHU);
            break;
        default:
            break;
        }
        }
    }
}

//metoda odpowiedzialna za ruchy gracza
void Gra::ruch_gracz() {
    //warunek pozwalajacy na zadanie innego kierunku ruchu i pozwalajacy go dokonac dopiero gdy bedzie taka mozliwosc do czasu, gdy zmienna liczba_wywolan_mozliwych_na_zmiane_ruchu bedzie rozna od zera
    if (gracz_obiekt -> liczba_wywolan_mozliwych_na_zmiane_ruchu != 0) {
        switch (gracz_obiekt -> temp) {
            case prawo:
                if (gracz_obiekt -> x_pole + 1 <= szerokosc - 1 && (!(mapa[gracz_obiekt -> y / 40][gracz_obiekt -> x / 40 + 1] == sciana || mapa[gracz_obiekt -> y / 40][gracz_obiekt -> x / 40 + 1] == wyjscie_duchow || mapa[(gracz_obiekt -> y + 39) / 40][gracz_obiekt -> x / 40 + 1] == sciana || mapa[(gracz_obiekt -> y + 39) / 40][gracz_obiekt -> x / 40 + 1] == wyjscie_duchow)))
                    gracz_obiekt -> kierunek_ruchu = gracz_obiekt -> temp;
                break;
            case lewo:
                if (gracz_obiekt -> x_pole - 1 >= 0 && (!(gracz_obiekt->x % 40 == 0 && (mapa[gracz_obiekt -> y / 40][gracz_obiekt -> x / 40 - 1] == sciana || mapa[gracz_obiekt -> y / 40][gracz_obiekt -> x / 40 - 1] == wyjscie_duchow || mapa[(gracz_obiekt -> y + 39) / 40][gracz_obiekt -> x / 40 - 1] == sciana || mapa[(gracz_obiekt -> y + 39) / 40][gracz_obiekt -> x / 40 - 1] == wyjscie_duchow))))
                    gracz_obiekt -> kierunek_ruchu = gracz_obiekt -> temp;
                break;
            case gora:
                if (gracz_obiekt -> y_pole - 1 >= 0 && (!(gracz_obiekt->y % 40 == 0 && (mapa[gracz_obiekt -> y / 40 - 1][gracz_obiekt -> x / 40] == sciana || mapa[gracz_obiekt -> y / 40 - 1][gracz_obiekt -> x / 40] == wyjscie_duchow || mapa[gracz_obiekt -> y / 40 - 1][(gracz_obiekt -> x + 39) / 40] == sciana || mapa[gracz_obiekt -> y / 40 - 1][(gracz_obiekt -> x + 39) / 40] == wyjscie_duchow))))
                    gracz_obiekt -> kierunek_ruchu = gracz_obiekt -> temp;
                break;
            case dol:
                if (gracz_obiekt -> y_pole + 1 <= wysokosc - 1 && (!(mapa[gracz_obiekt -> y / 40 + 1][gracz_obiekt -> x / 40] == sciana || mapa[gracz_obiekt -> y / 40 + 1][gracz_obiekt -> x / 40] == wyjscie_duchow || mapa[gracz_obiekt -> y / 40 + 1][(gracz_obiekt -> x + 39) / 40] == sciana || mapa[gracz_obiekt -> y / 40 + 1][(gracz_obiekt -> x + 39) / 40] == wyjscie_duchow)))
                    gracz_obiekt -> kierunek_ruchu = gracz_obiekt -> temp;
                break;
            default:
                break;
        }
        gracz_obiekt -> liczba_wywolan_mozliwych_na_zmiane_ruchu--;
    }
    if (gracz_obiekt->x % 40 == 0 && gracz_obiekt->y % 40 == 0) {
        gracz_obiekt->x_pole = gracz_obiekt->x / 40;
        gracz_obiekt->y_pole = gracz_obiekt->y / 40;
    }
    ruch temp = gracz_obiekt -> kierunek_ruchu;
    //zatrzymanie gracza, gdy spotyka sie ze sciana lub wyjsciem duchow
    switch (temp) {
    case prawo:
        if (gracz_obiekt -> x_pole + 1 <= szerokosc - 1 && (mapa[gracz_obiekt -> y / 40][gracz_obiekt -> x / 40 + 1] == sciana || mapa[gracz_obiekt -> y / 40][gracz_obiekt -> x / 40 + 1] == wyjscie_duchow || mapa[(gracz_obiekt -> y + 39) / 40][gracz_obiekt -> x / 40 + 1] == sciana || mapa[(gracz_obiekt -> y + 39) / 40][gracz_obiekt -> x / 40 + 1] == wyjscie_duchow))
            temp = brak;
        break;
    case lewo:
        if (gracz_obiekt -> x_pole - 1 >= 0 && (gracz_obiekt->x % 40 == 0 && (mapa[gracz_obiekt -> y / 40][gracz_obiekt -> x / 40 - 1] == sciana || mapa[gracz_obiekt -> y / 40][gracz_obiekt -> x / 40 - 1] == wyjscie_duchow || mapa[(gracz_obiekt -> y + 39) / 40][gracz_obiekt -> x / 40 - 1] == sciana || mapa[(gracz_obiekt -> y + 39) / 40][gracz_obiekt -> x / 40 - 1] == wyjscie_duchow)))
            temp = brak;
        break;
    case gora:
        if (gracz_obiekt -> y_pole - 1 >= 0 && (gracz_obiekt->y % 40 == 0 && (mapa[gracz_obiekt -> y / 40 - 1][gracz_obiekt -> x / 40] == sciana || mapa[gracz_obiekt -> y / 40 - 1][gracz_obiekt -> x / 40] == wyjscie_duchow || mapa[gracz_obiekt -> y / 40 - 1][(gracz_obiekt -> x + 39) / 40] == sciana || mapa[gracz_obiekt -> y / 40 - 1][(gracz_obiekt -> x + 39) / 40] == wyjscie_duchow)))
            temp = brak;
        break;
    case dol:
        if (gracz_obiekt -> y_pole + 1 <= wysokosc - 1 && (mapa[gracz_obiekt -> y / 40 + 1][gracz_obiekt -> x / 40] == sciana || mapa[gracz_obiekt -> y / 40 + 1][gracz_obiekt -> x / 40] == wyjscie_duchow || mapa[gracz_obiekt -> y / 40 + 1][(gracz_obiekt -> x + 39) / 40] == sciana || mapa[gracz_obiekt -> y / 40 + 1][(gracz_obiekt -> x + 39) / 40] == wyjscie_duchow))
            temp = brak;
        break;
    default:
        break;
    }
    //zmiana wspolrzednych gracza zgodnie z wybranym kierunkiem ruchu
    switch (temp) {
    case prawo:
        gracz_obiekt->x += SZYBKOSC_RUCHU;
        break;
    case lewo:
        gracz_obiekt->x -= SZYBKOSC_RUCHU;
        break;
    case gora:
        gracz_obiekt->y -= SZYBKOSC_RUCHU;
        break;
    case dol:
        gracz_obiekt->y += SZYBKOSC_RUCHU;
        break;
    default:
        break;
    gracz_obiekt -> kierunek_ruchu = temp;
    }

    //teleportacja gracza po przejsciu tunelu
    //DZIWNE WARUNKI I WSPOLRZEDNE, BO FUNKCJE OD RUCHU BADALY STAN PLANSZY POZA TABLICAMI!!!
    if (gracz_obiekt -> x < 0 + 40 - 40 / SZYBKOSC_RUCHU) { //z lewej na prawa strone
        gracz_obiekt -> x = (szerokosc - 2) * 40;
        gracz_obiekt -> x_pole = szerokosc - 2;
    }
    else if (gracz_obiekt -> x >= szerokosc * 40 - 40 - 40 / SZYBKOSC_RUCHU) { //z prawej na lewa strone
        gracz_obiekt -> x = 40;
        gracz_obiekt -> x_pole = 1;
    }
    else if (gracz_obiekt -> y < 0 + 40 - 40 / SZYBKOSC_RUCHU) { //z gory na dol
        gracz_obiekt -> y = (wysokosc - 2) * 40;
        gracz_obiekt -> y_pole = wysokosc - 2;
    }
    else if (gracz_obiekt -> y >= wysokosc * 40 - 40 - 40 / SZYBKOSC_RUCHU) { //z dolu na gore
        gracz_obiekt -> y = 40;
        gracz_obiekt -> y_pole = 1;
    }
}

//metoda odpowiedzialna za interakcje pomiedzy obiektami w grze
void Gra::interakcje() {
    //zdobycie punktu przez gracza
    if (mapa[gracz_obiekt->y_pole][gracz_obiekt->x_pole] == punkt) {
        gracz_obiekt -> punkty_kulki += 10;
        mapa[gracz_obiekt->y_pole][gracz_obiekt->x_pole] = puste_pole;
    }
    //zdobycie punktu przez gracza
    else if (mapa[gracz_obiekt->y_pole][gracz_obiekt->x_pole] == skrzyzowanie_punkt) {
        gracz_obiekt -> punkty_kulki += 10;
        mapa[gracz_obiekt->y_pole][gracz_obiekt->x_pole] = skrzyzowanie;
    }
    //zjedzenie przez gracza kulki, ktora umozliwia zjadanie duchow
    if (mapa[gracz_obiekt->y_pole][gracz_obiekt->x_pole] == zjadanie_duchow) {
        for (int i = 0; i < 3; i++) {
            //zmiana kierunku ruchu przeciwnika na przeciwny, gdy przeciwnicy moga byc zjadani
            if (!duch_obiekt[i]->mozliwosc_zjadania_przez_gracza) {
                duch_obiekt[i]->mozliwosc_zjadania_przez_gracza = true;
                switch (duch_obiekt[i]->kierunek_ruchu) {
                case dol:
                    duch_obiekt[i]->kierunek_ruchu = gora;
                    break;
                case gora:
                    duch_obiekt[i]->kierunek_ruchu = dol;
                    break;
                case lewo:
                    duch_obiekt[i]->kierunek_ruchu = prawo;
                    break;
                default:
                    duch_obiekt[i]->kierunek_ruchu = lewo;
                    break;
                }
            }
        }
        //utworzenie timera na czas mozliwosci zjadania duchow
        if (timer_mozliwosc_zjadania_duchow == NULL) {
            timer_mozliwosc_zjadania_duchow = new QTimer(this);
            connect(timer_mozliwosc_zjadania_duchow, SIGNAL(timeout()), this, SLOT(mozliwosc_zjadania_duchow()));
            timer_mozliwosc_zjadania_duchow->setSingleShot(true);
            timer_mozliwosc_zjadania_duchow->start(CZAS_NA_ZJADANIE);
        }
        //skasowanie i utworzenie timera, gdy podczas mozliwosci zjadania gracz zjadl znowu ta sama kulke
        else {
            delete timer_mozliwosc_zjadania_duchow;
            timer_mozliwosc_zjadania_duchow = new QTimer(this);
            connect(timer_mozliwosc_zjadania_duchow, SIGNAL(timeout()), this, SLOT(mozliwosc_zjadania_duchow()));
            timer_mozliwosc_zjadania_duchow->setSingleShot(true);
            timer_mozliwosc_zjadania_duchow->start(CZAS_NA_ZJADANIE);
        }
        mapa[gracz_obiekt->y_pole][gracz_obiekt->x_pole] = puste_pole;
    }
    //koniec gry - WYGRANA
    if (gracz_obiekt->punkty_kulki == punkty_do_zdobycia) {
        QMessageBox wygrana;
        wygrana.setText("GRATULACJE! Wygrałeś!\nTwoja liczba punktow to " + QString::number(gracz_obiekt -> punkty_kulki + gracz_obiekt -> punkty_duchy));
        wygrana.exec();
        this->close();
        this->timer->stop();
    }
    //koniec gry - PRZEGRANA
    if (gracz_obiekt->zycia == 0) {
        QMessageBox przegrana;
        przegrana.setText("Przegrałeś!\nTwoja liczba punktow to " + QString::number(gracz_obiekt -> punkty_kulki + gracz_obiekt -> punkty_duchy));
        przegrana.exec();
        this->close();
        this->timer->stop();
    }
    for (int i = 0; i < 3; i++) {
        //Gracz znajduje sie w zasiegu ducha i na odwrot
        if ((((gracz_obiekt -> x <= (duch_obiekt[i] -> x + ODLEGLOSC_ZJADANIA + 20)) && (gracz_obiekt -> x >= (duch_obiekt[i] -> x - ODLEGLOSC_ZJADANIA - 20))) && (duch_obiekt[i] -> y == gracz_obiekt -> y)) || ((((gracz_obiekt -> y <= (duch_obiekt[i] -> y + ODLEGLOSC_ZJADANIA + 20)) && (gracz_obiekt -> y >= (duch_obiekt[i] -> y - ODLEGLOSC_ZJADANIA - 20)))) && (duch_obiekt[i] -> x == gracz_obiekt -> x))) {
            if (duch_obiekt[i] -> mozliwosc_zjadania_przez_gracza) {
                duch_obiekt[i] -> x = duch_obiekt[i] -> x_pole_poczatkowe * 40;
                duch_obiekt[i] -> y = duch_obiekt[i] -> y_pole_poczatkowe * 40;
                duch_obiekt[i] -> x_pole = duch_obiekt[i] -> x_pole_poczatkowe;
                duch_obiekt[i] -> y_pole = duch_obiekt[i] -> y_pole_poczatkowe;
                duch_obiekt[i] -> aktywny = false;
                duch_obiekt[i] -> zjedzony = true;
                gracz_obiekt -> punkty_duchy =+ 100;
                switch (i) {
                case 0:
                    timer_aktywacja_duch0 = new QTimer(this);
                    connect(timer_aktywacja_duch0, SIGNAL(timeout()), this, SLOT(aktywacja_duch0()));
                    timer_aktywacja_duch0->setSingleShot(true);
                    timer_aktywacja_duch0->start(3000);
                    break;
                case 1:
                    timer_aktywacja_duch1 = new QTimer(this);
                    connect(timer_aktywacja_duch1, SIGNAL(timeout()), this, SLOT(aktywacja_duch1()));
                    timer_aktywacja_duch1->setSingleShot(true);
                    timer_aktywacja_duch1->start(3000);
                    break;
                case 2:
                    timer_aktywacja_duch2 = new QTimer(this);
                    connect(timer_aktywacja_duch2, SIGNAL(timeout()), this, SLOT(aktywacja_duch2()));
                    timer_aktywacja_duch2->setSingleShot(true);
                    timer_aktywacja_duch2->start(3000);
                    break;
                }
            }
            else {
                for (int j = 0; j < 3; j++) {
                    duch_obiekt[j] -> x = duch_obiekt[j] -> x_pole_poczatkowe * 40;
                    duch_obiekt[j] -> y = duch_obiekt[j] -> y_pole_poczatkowe * 40;
                    duch_obiekt[j] -> x_pole = duch_obiekt[j] -> x_pole_poczatkowe;
                    duch_obiekt[j] -> y_pole = duch_obiekt[j] -> y_pole_poczatkowe;
                    duch_obiekt[j] -> aktywny = false;
                }
                gracz_obiekt -> x = gracz_obiekt -> x_pole_poczatkowe * 40;
                gracz_obiekt -> y = gracz_obiekt -> y_pole_poczatkowe * 40;
                gracz_obiekt -> x_pole = gracz_obiekt -> x_pole_poczatkowe;
                gracz_obiekt -> y_pole = gracz_obiekt -> y_pole_poczatkowe;
                gracz_obiekt -> zycia -= 1;
                //QPixmap *mapa_obrazek = new QPixmap(szerokosc*40, wysokosc*40);
                //rysowanie_postaci(mapa_obrazek);
                //delete mapa_obrazek;
                Sleep(3000);
                timer_aktywacja = new QTimer(this);
                connect(timer_aktywacja, SIGNAL(timeout()), this, SLOT(aktywacja()));
                timer_aktywacja->start(3000);
                break;
            }
        }
    }
}

//nadpisana metoda odpowiedzialna za obsluge sterowania z klawiatury - strzalkami
void Gra::keyPressEvent(QKeyEvent *event) {
    switch (event->key())
    {
    case Qt::Key_Left:
        gracz_obiekt -> temp = lewo;
        break;
    case Qt::Key_Right:
        gracz_obiekt -> temp =  prawo;
        break;
    case Qt::Key_Down:
        gracz_obiekt -> temp = dol;
        break;
    case Qt::Key_Up:
        gracz_obiekt -> temp = gora;
        break;
    default:
        break;
    }
    gracz_obiekt -> liczba_wywolan_mozliwych_na_zmiane_ruchu = LICZBA_WYWOLAN;
//    switch (temp) {
//    case prawo:
//        if (gracz_obiekt -> x_pole + 1 <= szerokosc - 1 && (!(mapa[gracz_obiekt -> y / 40][gracz_obiekt -> x / 40 + 1] == sciana || mapa[gracz_obiekt -> y / 40][gracz_obiekt -> x / 40 + 1] == wyjscie_duchow || mapa[(gracz_obiekt -> y + 39) / 40][gracz_obiekt -> x / 40 + 1] == sciana || mapa[(gracz_obiekt -> y + 39) / 40][gracz_obiekt -> x / 40 + 1] == wyjscie_duchow)))
//            gracz_obiekt -> kierunek_ruchu = temp;
//        break;
//    case lewo:
//        if (gracz_obiekt -> x_pole - 1 >= 0 && (!(gracz_obiekt->x % 40 == 0 && (mapa[gracz_obiekt -> y / 40][gracz_obiekt -> x / 40 - 1] == sciana || mapa[gracz_obiekt -> y / 40][gracz_obiekt -> x / 40 - 1] == wyjscie_duchow || mapa[(gracz_obiekt -> y + 39) / 40][gracz_obiekt -> x / 40 - 1] == sciana || mapa[(gracz_obiekt -> y + 39) / 40][gracz_obiekt -> x / 40 - 1] == wyjscie_duchow))))
//            gracz_obiekt -> kierunek_ruchu = temp;
//        break;
//    case gora:
//        if (gracz_obiekt -> y_pole - 1 >= 0 && (!(gracz_obiekt->y % 40 == 0 && (mapa[gracz_obiekt -> y / 40 - 1][gracz_obiekt -> x / 40] == sciana || mapa[gracz_obiekt -> y / 40 - 1][gracz_obiekt -> x / 40] == wyjscie_duchow || mapa[gracz_obiekt -> y / 40 - 1][(gracz_obiekt -> x + 39) / 40] == sciana || mapa[gracz_obiekt -> y / 40 - 1][(gracz_obiekt -> x + 39) / 40] == wyjscie_duchow))))
//            gracz_obiekt -> kierunek_ruchu = temp;
//        break;
//    case dol:
//        if (gracz_obiekt -> y_pole + 1 <= wysokosc - 1 && (!(mapa[gracz_obiekt -> y / 40 + 1][gracz_obiekt -> x / 40] == sciana || mapa[gracz_obiekt -> y / 40 + 1][gracz_obiekt -> x / 40] == wyjscie_duchow || mapa[gracz_obiekt -> y / 40 + 1][(gracz_obiekt -> x + 39) / 40] == sciana || mapa[gracz_obiekt -> y / 40 + 1][(gracz_obiekt -> x + 39) / 40] == wyjscie_duchow)))
//            gracz_obiekt -> kierunek_ruchu = temp;
//        break;
//    default:
//        break;
//    }
}

//metoda odpowiedzialna za sekwencyjna aktywacje duchow gdy znajduja sie na respawnie
void Gra::aktywacja() {
    duch_obiekt[numer_ducha_do_aktywacji]->aktywny = true;
    if (numer_ducha_do_aktywacji == 2)
        timer_aktywacja -> stop();
    numer_ducha_do_aktywacji++;
    if (numer_ducha_do_aktywacji >= 3)
        numer_ducha_do_aktywacji = 0;
}

//metoda wylaczajaca mozliwosc zjadania duchow i ustawiajaca ich ruchy z ucieczki na poscig
void Gra::mozliwosc_zjadania_duchow() {
    for (int i = 0; i < 3; i++) {
        duch_obiekt[i]->mozliwosc_zjadania_przez_gracza = false;
        switch (duch_obiekt[i]->kierunek_ruchu) {
        case dol:
            duch_obiekt[i]->kierunek_ruchu = gora;
            break;
        case gora:
            duch_obiekt[i]->kierunek_ruchu = dol;
            break;
        case lewo:
            duch_obiekt[i]->kierunek_ruchu = prawo;
            break;
        default:
            duch_obiekt[i]->kierunek_ruchu = lewo;
            break;
        }
    }
    delete timer_mozliwosc_zjadania_duchow;
    timer_mozliwosc_zjadania_duchow = NULL;
}

//aktywacja ducha znajdujacego sie pod adresem duch_obiekt[0] po zjedzeniu minieciu czasu w timerze timer_aktywacja_duch0
void Gra::aktywacja_duch0() {
    duch_obiekt[0] -> zjedzony = false;
    delete timer_aktywacja_duch0;
}

//aktywacja ducha znajdujacego sie pod adresem duch_obiekt[1] po zjedzeniu minieciu czasu w timerze timer_aktywacja_duch1
void Gra::aktywacja_duch1() {
    duch_obiekt[1] -> zjedzony = false;
    delete timer_aktywacja_duch1;
}

//aktywacja ducha znajdujacego sie pod adresem duch_obiekt[2] po zjedzeniu minieciu czasu w timerze timer_aktywacja_duch2
void Gra::aktywacja_duch2() {
    duch_obiekt[2] -> zjedzony = false;
    delete timer_aktywacja_duch2;
}
