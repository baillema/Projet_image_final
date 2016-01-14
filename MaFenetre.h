#ifndef MAFENETRE_H
#define MAFENETRE_H

#include <QApplication>
#include <QPushButton>
#include <QWidget>

class MaFenetre : public QWidget //Héritage de QWidget
{
public:
    MaFenetre();
private:
    QPushButton *m_bouton;
};

#endif // MAFENETRE_H

