/********************************************************************************
** Form generated from reading UI file 'gra.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRA_H
#define UI_GRA_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Gra
{
public:
    QLabel *imageLabel;
    QLabel *zycia;
    QLabel *punkty;
    QLabel *czas_zjadania;

    void setupUi(QWidget *Gra)
    {
        if (Gra->objectName().isEmpty())
            Gra->setObjectName(QStringLiteral("Gra"));
        Gra->resize(627, 445);
        imageLabel = new QLabel(Gra);
        imageLabel->setObjectName(QStringLiteral("imageLabel"));
        imageLabel->setGeometry(QRect(0, 0, 191, 101));
        zycia = new QLabel(Gra);
        zycia->setObjectName(QStringLiteral("zycia"));
        zycia->setGeometry(QRect(520, 10, 47, 31));
        punkty = new QLabel(Gra);
        punkty->setObjectName(QStringLiteral("punkty"));
        punkty->setGeometry(QRect(520, 40, 47, 31));
        czas_zjadania = new QLabel(Gra);
        czas_zjadania->setObjectName(QStringLiteral("czas_zjadania"));
        czas_zjadania->setGeometry(QRect(520, 80, 47, 13));

        retranslateUi(Gra);

        QMetaObject::connectSlotsByName(Gra);
    } // setupUi

    void retranslateUi(QWidget *Gra)
    {
        Gra->setWindowTitle(QApplication::translate("Gra", "Gra", Q_NULLPTR));
        imageLabel->setText(QString());
        zycia->setText(QApplication::translate("Gra", "\305\273ycia", Q_NULLPTR));
        punkty->setText(QApplication::translate("Gra", "Punkty", Q_NULLPTR));
        czas_zjadania->setText(QApplication::translate("Gra", "Zjadanie", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Gra: public Ui_Gra {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRA_H
