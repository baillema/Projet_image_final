#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"traiterImage.h"
#include<QtGui>
#include<QMessageBox>
#include<QFileDialog>
#include<QFile>
#include<QFileInfo>
#include<QSize>
#include<QImage>
#include<opencv/cv.h>
#include<opencv/highgui.h>
#include<opencv2/video/background_segm.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<iostream>

using namespace cv;
using namespace std;



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    creer_shortcut();
    creer_connexions();

}

MainWindow::~MainWindow()
{
    delete ui;

}


void MainWindow::creer_shortcut()
{
    ui->actionOuvrir->setShortcut(tr("Ctrl+O"));
    ui->actionEnregistrer->setShortcut(tr("Ctrl+S"));
    ui->actionFermer->setShortcut(tr("Ctrl+Q"));
}

void MainWindow::creer_connexions()
{
    QObject::connect(ui->actionOuvrir, SIGNAL(triggered()), this, SLOT(ouvrir()));
    QObject::connect(ui->actionFermer, SIGNAL(triggered()), this, SLOT(fermer()));
    QObject::connect(ui->actionEnregistrer, SIGNAL(triggered()), this, SLOT(enregistrer()));

}

void MainWindow::loadImage()
{
    QSize size(ui->label_image->width(), ui->label_image->height());
    QImage image2 = image1.scaled(size, Qt::KeepAspectRatio);
    ui->label_image->setPixmap(QPixmap::fromImage(image2));

}



void MainWindow::ouvrir()
{
    QString fichier = QFileDialog::getOpenFileName(this, "Ouvrir un fichier", QString(), "Images (*.png *.gif *.jpg *.jpeg)");
    if(fichier!=NULL)
    {
        QImage image(fichier);
        image1 = image;
        if (image1.isNull())
        {
            QMessageBox::information(this, "MainWindow", tr("ne peut pas être chargé").arg(fichier));
            return;
        }
        int fact = image1.depth()/8;
        traiterImage = new TraiterImage(image1.height(), fact*image1.width());
        this->loadImage();

        //Afficher les histogrammes
        const char*c = fichier.toStdString().c_str();
        Mat src = imread(c);
        vector<Mat>tab = histogramme(src);
        for(int i = 0; i<3; i++){
            if(i == 0)
            {
                QPixmap image_histo = Mat2QPixmap(tab[i]);
                int fact1 = image_histo.depth()/8;
                traiter_histo = new TraiterImage(image_histo.height(), fact1*image_histo.width());
                QSize size(ui->label_histo1->width(), ui->label_histo1->height());
                QImage image2 = image_histo.toImage();
                image2.scaled(size, Qt::KeepAspectRatio);
                ui->label_histo1->setPixmap(QPixmap::fromImage(image2));

            }
            else{
                if(i == 1)
                {
                    QPixmap image_histo = Mat2QPixmap(tab[i]);
                    int fact1 = image_histo.depth()/8;
                    traiter_histo = new TraiterImage(image_histo.height(), fact1*image_histo.width());
                    QSize size(ui->label_histo2->width(), ui->label_histo2->height());
                    QImage image2 = image_histo.toImage();
                    image2.scaled(size, Qt::KeepAspectRatio);
                    ui->label_histo2->setPixmap(QPixmap::fromImage(image2));

                }
                else
                {
                    QPixmap image_histo = Mat2QPixmap(tab[i]);
                    int fact1 = image_histo.depth()/8;
                    traiter_histo = new TraiterImage(image_histo.height(), fact1*image_histo.width());
                    QSize size(ui->label_histo2->width(), ui->label_histo2->height());
                    QImage image2 = image_histo.toImage();
                    image2.scaled(size, Qt::KeepAspectRatio);
                    ui->label_histo3->setPixmap(QPixmap::fromImage(image2));

                }
            }





        }




    }

   else{
          QMessageBox::information(this, "Fichier", "Vous n'avez rien sélectiooné\n");


    }

}

void MainWindow::enregistrer()
{
    QString fichier = QFileDialog::getSaveFileName(this, "Enregistrer le fichier", fileName, "Images (*.png *.gif *.jpg *.jpeg)");
    QFile file(fichier);
    if(!file.open(QIODevice::WriteOnly)){
        return;
    }
    QImage image = this->image1;
    QString extension = QFileInfo(fichier).suffix();
    image.save(fichier);

}

void MainWindow::about()
{

}

void MainWindow::fermer()
{
    qApp->exit();
}

QPixmap MainWindow::IPlImage2QImage(const IplImage *newImage)
{

    QPixmap monPixmap;
    QImage qtemp;
    if (newImage && cvGetSize(newImage).width>0)
    {
        int x;
        int y;
        char*data = newImage->imageData;
        qtemp = QImage(newImage->width, newImage->height, QImage::Format_RGB32);
        for(y = 0 ; y<newImage->height ; y++, data+=newImage->widthStep)
        {
            for(x = 0; x<newImage->width; x++)
            {
                uint*p = (uint*)qtemp.scanLine(y)+x;
                *p = qRgb(data[x*newImage->nChannels+2],data[x*newImage->nChannels+1],data[x*newImage->nChannels]);
            }

        }



    }

    monPixmap = QPixmap::fromImage(qtemp);

    return monPixmap;
}



vector<Mat> MainWindow::histogramme(Mat &img)
{
    int bins = 256;  //nombre de bacs
    int nc = img.channels();//nombre de channels

    vector<Mat>hist(nc);  //tableau d'histogrammes

    //initialiser l'histogramme
    for(int i = 0; i<hist.size();i++)
    {
        hist[i] = Mat::zeros(1, bins, CV_32SC1);

    }

    for(int i = 0; i<img.rows; i++)
    {
        for(int j=0;j<img.cols;j++)
        {
            for(int k = 0; k<nc; k++)
            {
                uchar val;
                if(nc == 1){
                    val = img.at<uchar>(i,j);
                }
                else{
                    val = img.at<Vec3b>(i,j)[k];
                }

                hist[k].at<int>(val) += 1;


            }
        }
    }

    int hmax[3] = {0,0,0};
    for(int i=0;i<nc;i++)
    {
        for(int j=0;j<bins-1;j++)
        {
           if( hist[i].at<int>(j) > hmax[i] ){

               hmax[i] = hist[i].at<int>(j);
           }
           else{

           }
        }
    }


    //Nouvel Ajout a modifier aprés
    //QLabel *myimage = new QLabel();
   // myimage->setBackgroundRole(QPalette::Dark);
   // myimage->setScaledContents(true);




    //const char*wname[3] = {"blue", "green", "red"};
    Scalar colors[3] = {Scalar(255,0,0), Scalar(0,255,0), Scalar(0,0,255)};
    vector<Mat> canavas(nc);
    for(int i =0; i<nc; i++)
    {
        canavas[i] = Mat::ones(125, bins, CV_8UC3);
        for(int j=0, rows = canavas[i].rows; j<bins-1; j++ )
        {
            Scalar s;
            if(nc == 1)
                s = Scalar(200,200,200);
            else
                s = colors[i];

            line(
                        canavas[i],
                        Point(j,rows),
                        Point(j,rows-(hist[i].at<int>(j)*rows/hmax[i])),
                        s,//nc = 1  ? Scalar(200,200,200): colors[i],
                        1, 8, 0
                        );
        }



      // imshow(nc ==1 ? "value" : wname[i], canavas[i]);}
      //  QLabel label(ui->Tab1);
       //label.setPixmap(Mat2QPixmap(canavas[i]));

         //QPixmap pix = Mat2QPixmap(canavas[i]);
       //  int fact1 = pix.depth()/8;
       //  traiterImage = new TraiterImage(pix.height(), fact1*pix.width() );
  //      myimage->setPixmap(pix);
    //    ui->verticalLayout->addWidget(myimage);
        //load image histogramme



    }


    return canavas;




}


QPixmap MainWindow::Mat2QPixmap(const Mat &mat)
{
    Mat rgb;
    QPixmap p;
    cvtColor(mat, rgb, CV_BGR2RGB);
    p.convertFromImage(QImage((const unsigned char*)(rgb.data), rgb.cols, rgb.rows, QImage::Format_RGB888));
    return p;

}

void MainWindow::afficher_histogramme()
{


}

