/* 
 * Analyse d'images TP4
 * IRM2016 ReVA
 * File:   CustomPixel.h
 * Author: Amandine ROGER
 */
#ifndef CUSTOMPIXEL_H
#define CUSTOMPIXEL_H

#include <opencv2/core/types_c.h>
#include <opencv2/core/core_c.h>

#include <stddef.h>

using namespace std;

class CustomPixel {
public:
    CustomPixel(int valeur, CustomPixel * voisinHaut, CustomPixel * voisinGauche);

    void SetEtiquette(int etiquette);
    int GetEtiquette() const;
    
    //Mapping
    static CustomPixel** imageToCustomPixelArray(IplImage * image);
    static IplImage * CustomPixelArrayToImage(CustomPixel** pixels, int nbLignes, int nbColonnes);
    char GetValeur() const;
    
private:
    char valeur;
    int etiquette;
    CustomPixel * vHaut;
    CustomPixel * vGauche;

};

#endif /* CUSTOMPIXEL_H */

