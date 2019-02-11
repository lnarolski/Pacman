/********************************************************************************
** Form generated from reading UI file 'pacman.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PACMAN_H
#define UI_PACMAN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Pacman
{
public:
    QWidget *centralWidget;
    QPushButton *graj;
    QComboBox *wybor_mapy;
    QLabel *label;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Pacman)
    {
        if (Pacman->objectName().isEmpty())
            Pacman->setObjectName(QStringLiteral("Pacman"));
        Pacman->resize(300, 150);
        Pacman->setMinimumSize(QSize(300, 150));
        Pacman->setMaximumSize(QSize(300, 150));
        centralWidget = new QWidget(Pacman);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        graj = new QPushButton(centralWidget);
        graj->setObjectName(QStringLiteral("graj"));
        graj->setGeometry(QRect(190, 70, 71, 31));
        wybor_mapy = new QComboBox(centralWidget);
        wybor_mapy->setObjectName(QStringLiteral("wybor_mapy"));
        wybor_mapy->setGeometry(QRect(30, 70, 151, 31));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(70, 40, 71, 16));
        Pacman->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(Pacman);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        Pacman->setStatusBar(statusBar);

        retranslateUi(Pacman);

        QMetaObject::connectSlotsByName(Pacman);
    } // setupUi

    void retranslateUi(QMainWindow *Pacman)
    {
        Pacman->setWindowTitle(QApplication::translate("Pacman", "Pacman", Q_NULLPTR));
        graj->setText(QApplication::translate("Pacman", "Graj", Q_NULLPTR));
        label->setText(QApplication::translate("Pacman", "Wybierz map\304\231:", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Pacman: public Ui_Pacman {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PACMAN_H
