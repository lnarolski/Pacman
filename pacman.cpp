#include "pacman.h"
#include "ui_pacman.h"
#include "io.h"
#include "gra.h"

Pacman::Pacman(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Pacman)
{
    ui->setupUi(this);
    okno_gry = NULL;
    int SzukamDalej;
    _finddata_t danePliku;
    //wyszukiwanie plikow z mapami o rozszerzeniu *.txt w folderze "maps"
    long uchwyt = _findfirst( "maps/*.txt", & danePliku );
    if (uchwyt != -1) {
        ui->wybor_mapy->addItem(danePliku.name);
        while ((SzukamDalej = _findnext( uchwyt, & danePliku )) != -1) {
            ui->wybor_mapy->addItem(danePliku.name);
        }
    }
}

Pacman::~Pacman()
{
    delete ui;
    delete okno_gry;
}

//uruchomienie gry na wybranej planszy po nacisnieciu przycisku "Graj"
void Pacman::on_graj_clicked()
{
    if (okno_gry != NULL && okno_gry -> close()) { //skasowanie starego okna z gra, gdy gra jest uruchamiana kolejny raz
        delete okno_gry;
        okno_gry = NULL;
    }
    if (okno_gry == NULL) {
        if (ui->wybor_mapy->count() != 0) {
            okno_gry = new Gra(ui->wybor_mapy->currentText());
        }
    }
}
