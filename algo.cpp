#include <iostream>
#include <QFile>
#include <QFileDialog>
#include <QImage>
#include <QColor>
#include <QLabel>
#include <QRgb>
#include <QApplication>
#include <QMessageBox>

using namespace std;

int algo(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /*
    QImage img = QImage("poke.png");

    cout << img.depth() << endl;
    cout << img.allGray() << endl;
    */

    QString fileName = QFileDialog::getOpenFileName(NULL, "Ouvrir un fichier", QString(), "Images (*.png *.gif *.jpg *.jpeg)");

    QImage image(fileName);
    QImage imageCpy(fileName);

    if(image.isNull())
    {
        QMessageBox::warning(NULL, QObject::tr("Erreur"), QObject::tr("Image non valide"));
        return 0;
    }

    QLabel label;
    label.resize(image.size()/2);
    label.setPixmap(QPixmap::fromImage(image));
    label.setWindowTitle(QObject::tr("Image"));
    label.show();

    //Image en inversé
    //imageCpy.invertPixels();

    //Image en noir et blanc
    /*
    for(int i = 0; i<imageCpy.width(); i++)
    {
        for(int j = 0; j<imageCpy.height(); j++)
        {
            QRgb rgb = imageCpy.pixel(i,j);
            //Calcul par moyenne
            int res = (qGreen(rgb)+qRed(rgb)+qBlue(rgb))/3;
            int resM = max(max(qGreen(rgb),qRed(rgb)),qBlue(rgb));

            imageCpy.setPixel(i,j,qRgb(resM,resM,resM));
        }
    }
    */

    //Flou de niveau 1
    /*
    QImage imageCpy1(fileName);

    for(int i = 1; i<imageCpy.width()-1;i++)
    {
        for(int j = 1; j<imageCpy.height()-1;j++)
        {
            QRgb gauche = imageCpy.pixel(i-1,j);
            QRgb droite = imageCpy.pixel(i+1,j);
            QRgb haut = imageCpy.pixel(i,j+1);
            QRgb bas = imageCpy.pixel(i,j-1);

            int resR = (qRed(haut)+qRed(bas)+qRed(gauche)+qRed(droite))/4;
            int resG = (qGreen(haut)+qGreen(bas)+qGreen(gauche)+qGreen(droite))/4;
            int resB = (qBlue(haut)+qBlue(bas)+qBlue(gauche)+qBlue(droite))/4;

            imageCpy1.setPixel(i,j, qRgb(resR, resG, resB));
        }
    }

    imageCpy = imageCpy1;
    */

    //Flou de niveau 2
    QImage imageCpy1(fileName);

    for(int i = 2; i<imageCpy.width()-2;i++)
    {
        for(int j = 2; j<imageCpy.height()-2;j++)
        {
            QRgb gauche = imageCpy.pixel(i-1,j);
            QRgb droite = imageCpy.pixel(i+1,j);
            QRgb haut = imageCpy.pixel(i,j+1);
            QRgb bas = imageCpy.pixel(i,j-1);

            QRgb gauche2 = imageCpy.pixel(i-2,j);
            QRgb droite2 = imageCpy.pixel(i+2,j);
            QRgb haut2 = imageCpy.pixel(i,j+2);
            QRgb bas2 = imageCpy.pixel(i,j-2);

            QRgb gaucheD = imageCpy.pixel(i-1,j+1);
            QRgb droiteD = imageCpy.pixel(i+1,j+1);
            QRgb hautD = imageCpy.pixel(i+1,j+1);
            QRgb basD = imageCpy.pixel(i+1,j-1);


            int resR = (2*qRed(haut)+2*qRed(bas)+2*qRed(gauche)+qRed(droite)+qRed(haut2)+qRed(bas2)+qRed(gauche2)+qRed(droite2)+qRed(droiteD)+qRed(hautD)+qRed(basD)+qRed(gaucheD)+qRed(droiteD))/16;
            int resG = (2*qGreen(haut)+2*qGreen(bas)+2*qGreen(gauche)+qGreen(droite)+qGreen(haut2)+qGreen(bas2)+qGreen(gauche2)+qGreen(droite2)+qGreen(droiteD)+qGreen(hautD)+qGreen(basD)+qGreen(gaucheD)+qGreen(droiteD))/16;
            int resB = (2*qBlue(haut)+2*qBlue(bas)+2*qBlue(gauche)+qBlue(droite)+qBlue(haut2)+qBlue(bas2)+qBlue(gauche2)+qBlue(droite2)+qBlue(droiteD)+qBlue(hautD)+qBlue(basD)+qBlue(gaucheD)+qBlue(droiteD))/16;

            imageCpy1.setPixel(i,j, qRgb(resR, resG, resB));
        }
    }

    imageCpy = imageCpy1;




    QString enregistre = QFileDialog::getSaveFileName(NULL, "Enregistrer un fichier", QString(), "Images (*.png *.gif *.jpg *.jpeg)");

    imageCpy.save(enregistre);


    return a.exec();
}
