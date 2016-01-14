#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QImage>
#include<QLabel>
#include<opencv/cv.h>
#include<opencv/highgui.h>
#include<opencv2/video/background_segm.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;


#include"traiterImage.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
protected slots:
    //Les slots
    void ouvrir();  //ouvrir l'image
    void enregistrer();  //enregistrer l'image
    void about();
    void fermer();  //fermer l'image


private:

    QPixmap Mat2QPixmap(const Mat&);  //convertir un Mat en QPixmap
    void afficher_histogramme();      //Afficher les histogrammes de ccouleurs de l'image
    vector<Mat> histogramme(Mat&);  //Créer l'histogramme de couleurs de l'image
    void creer_connexions();       //créer les connexions
    void loadImage();             //charger l'image
    void creer_shortcut();        //creer les raccourcis
    QPixmap IPlImage2QImage(const IplImage*);  //Convertir une IplImage en QImage
    TraiterImage*traiterImage;  //traitement de l'image
    TraiterImage*traiter_histo;//Traitement des histgrammes
    QString fileName; //fichier pour ouverture de l'image
    QImage image1;  //image a ouvrir
    Ui::MainWindow *ui;//fenêtre principale
};

#endif // MAINWINDOW_H
