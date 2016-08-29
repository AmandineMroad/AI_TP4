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

void CustomPixel::SetValeur(char valeur) {
    this->valeur = valeur;
}

CustomPixel* CustomPixel::GetVGauche() const {
    return vGauche;
}

CustomPixel* CustomPixel::GetVHaut() const {
    return vHaut;
}

void CustomPixel::SetEtiquette(int etiquette) {
    this->etiquette = etiquette;
    this->valeur = 0;//TODO delete
}

int CustomPixel::GetEtiquette() const {
    return etiquette;
}

 /**
 * Convertit une image en un tableau de CustomPixel
 * @param image l'image à convertir
 * @return le tableau de CustomPixel correspondant à l'image
 */
CustomPixel*** CustomPixel::imageToCustomPixelArray(IplImage * image){
    cout<<"imageToCustomPixelArray begins"<<endl;
    int nbCol = image->width;
    int nbLigne = image->height;
    
    CustomPixel*** pixels = new CustomPixel**[nbLigne];
    
    int i,j;
    CustomPixel * haut,
            * gauche;
    
    for (i=0; i<nbLigne; i++){
        pixels[i] = new CustomPixel*[nbCol];
        for (j=0; j<nbCol; j++){
            //première ligne (pas de voisin haut)
            if (i == 0) {
                haut = NULL;
            } else {
                haut = pixels[i-1][j];
            }
            
            //première colonne (pas de voisin gauche)
            if (j == 0) {
                gauche = NULL;
            } else {
                gauche = pixels[i][j-1];
            }
            
            //Création du pixel
            pixels[i][j] = new CustomPixel(cvGet2D(image,i,j).val[0], haut, gauche);
        }
    }
    cout<<"imageToCustomPixelArray ends"<<endl;
    return pixels;
}

IplImage* CustomPixel::CustomPixelArrayToImage(CustomPixel** pixels, int nbLignes, int nbColonnes){
    IplImage* img = cvCreateImage(cvSize(nbLignes,nbColonnes),IPL_DEPTH_8U,1);
    cout<<"CustomPixelArrayToImage begins"<<endl;
    int i,j,offset;
    for (i=0; i<nbLignes ; i++){
        offset = i*nbColonnes;
        for(j=0; j<nbColonnes; j++){
            img->imageData[j+offset] = pixels[i][j].GetValeur();
        }
    }
    cout<<"CustomPixelArrayToImage ends"<<endl;
    return img;
}

bool CustomPixel::isNotNull(){
    if (this == NULL) return false;
    else return true;
}

ostream& CustomPixel::operator <<(ostream& flux){ 
    flux<<"Pixel : {valeur: "<<this->GetValeur()
            <<", e: "<<this->GetEtiquette()
            <<", vHaut: "<<this->GetVHaut()
            <<", vGau: "<<this->GetVGauche();
    return flux;    
}
