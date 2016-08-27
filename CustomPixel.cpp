/* 
 * Analyse d'images TP4
 * IRM2016 ReVA
 * File:   CustomPixel.cpp
 * Author: Amandine ROGER
 */

#include "CustomPixel.h"

CustomPixel::CustomPixel(int couleur, CustomPixel * voisinHaut, CustomPixel * voisinGauche){
 this->valeur = couleur;
 this->vHaut = voisinHaut;
 this->vGauche = voisinGauche;
 this->etiquette = -1;
}

char CustomPixel::GetValeur() const {
    return valeur;
}

CustomPixel* CustomPixel::GetVGauche() const {
    return vGauche;
}

CustomPixel* CustomPixel::GetVHaut() const {
    return vHaut;
}

void CustomPixel::SetEtiquette(int etiquette) {
    this->etiquette = etiquette;
}

int CustomPixel::GetEtiquette() const {
    return etiquette;
}

 /**
 * Convertit une image en un tableau de CustomPixel
 * @param image l'image à convertir
 * @return le tableau de CustomPixel correspondant à l'image
 */
CustomPixel** CustomPixel::imageToCustomPixelArray(IplImage * image){
    int width = image->width;
    int height = image->height;
    
    CustomPixel** pixels = new CustomPixel*[width];
    
    int i,j;
    CustomPixel * haut,
            * gauche;
    
    for (i=0; i<width; i++){
        for (j=0; j<height; j++){
            //Si pixel de début de ligne
            if (i == 0) {
                gauche = NULL;
            } else {
                gauche = &pixels[i-1][j];
            }
            
            //si pixel de début de colonne
            if (j == 0) {
                haut = NULL;
            } else {
                haut = &pixels[i][j-1];
            }
            
            //Création du pixel
            pixels[i][j] = *(new CustomPixel(cvGet2D(image,i,j).val[0], haut, gauche));
        }
    }
    
    return pixels;
}

IplImage* CustomPixelArrayToImage(CustomPixel** pixels, int nbLignes, int nbColonnes){
    IplImage* img = cvCreateImage(cvSize(nbLignes,nbColonnes),IPL_DEPTH_8U,1);
    
    int i,j,offset;
    for (i=0; i<nbLignes ; i++){
        offset = i*nbColonnes;
        for(j=0; j<nbColonnes; j++){
            img->imageData[j+offset] = pixels[i][j].GetValeur();
        }
    }
    
    return img;
}