#ifndef TRAITERIMAGE_H
#define TRAITERIMAGE_H
#include<iostream>
#include<QImage>
using namespace std;

typedef unsigned char byte;
typedef double PIXEL;
class TraiterImage

{
private:
    //***********Attributs********************

    byte**m_Image;  //matrice de l'image
    PIXEL**m_normex; //matrice du masque horizontal(x)
    PIXEL**m_normey; //matrice du masque vertical(y)
    PIXEL**m_norme;  //matrice de la norme du gradient
    byte**m_buffer;//buffer de l'image;
    int m_width;  //longeur de l'image
    int m_heigth;//hauteur de l'image

    //************Méthodes************************

    void Allouer();
    void Liberer();

public:
    TraiterImage();
    TraiterImage(int, int);
    ~TraiterImage();
    int Width();
    int Heigth();
    void affiche();
};


#endif // TRAITERIMAGE_H
