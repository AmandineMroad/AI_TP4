#include <cstdlib>
#include <opencv2/core/types_c.h>
#include <opencv2/core/core_c.h>
#include <opencv2/highgui/highgui_c.h>
#include <iostream>
#include <algorithm>


#include "CustomPixel.h"


const char * const W_BASE = "Image initiale";
const char * const W_ETIQ = "Image etiquetée";

using namespace std;

IplImage * imageBase;

bool isValidCommand(int nbArgs, char** args);
inline IplImage* getEtiquetage();
CustomPixel*** algoEtiquetage(CustomPixel*** pixels);

int main(int argc, char** argv) {
    //verification des arguments du proramme
    if (!isValidCommand(argc, argv)) return EXIT_FAILURE;

    //initialisation des fenetres
    cvNamedWindow(W_BASE);
    cvNamedWindow(W_ETIQ);

    //Affichage des images
    cvShowImage(W_BASE, imageBase);
    cvShowImage(W_ETIQ, getEtiquetage());

    //Attente d'une entrée clavier
    cvWaitKey();

    //fermeture du programme
    cvDestroyAllWindows();
    return EXIT_SUCCESS;
}

/**
 * Verifie la validité des arguments
 * @param nbArgs le nombre d'arguments passés
 * @param args les arguments
 * @return true si les arguments sont valides, false sinon;
 */
bool isValidCommand(int nbArgs, char** args) {
    //Verification du nombre d'arguments
    if (nbArgs != 2) {
        cerr << "Utilisation: \n\t etiquetage <nom_fichier_image>" << endl;
        return false;
    }

    //Chargement des images
    IplImage * tmp = cvLoadImage(args[1], CV_LOAD_IMAGE_GRAYSCALE);
    if (tmp == 0) {
        cerr << "Impossible de lire : " << args[1] << "\n";
        return false;
    }
    imageBase = cvCloneImage(tmp);
    return true;
}

int** equivTable = new int*;

CustomPixel*** algoEtiquetage(CustomPixel*** pixels) {
    cout<<"algoEtiquetage begins"<<endl;
    int nbLigne = imageBase->height;
    int nbCol = imageBase->width;

    CustomPixel* pixel,
            * vh,
            * vg;

    int currentEtiq = 0, eh, eg;
    int i, j;
    //Parcours du tableau de CustomPixels
    for (i = 0; i < nbLigne; i++) {
        for (j = 0; j < nbCol; j++) {
            pixel = pixels[i][j];
            //Traitement si le pixel n'appartient pas au fond (non blanc)
            if (pixel->GetValeur() > 250) {

            } else {
                //Récupération des voisins haut et gauche
                vh = pixel->GetVHaut();
                vg = pixel->GetVGauche();

                //Si le pixel a deux voisins
                if (vh->isNotNull() && vg->isNotNull()) {
                    eh = vh->GetEtiquette();
                    eg = vg->GetEtiquette();

                    //Les deux voisins ont la même etiquette
                    if (eh == eg) {
                        if (eh == -1) { //les étiquettes sont nulles
                            pixel->SetEtiquette(currentEtiq++);
                        } else {
                            pixel->SetEtiquette(eh);
                        }
                        
                    }
                    //Les voisins ont des etiquettes différentes
                    else {
                        //le voisin haut n'a pas d'étiquette
                        if (eh == -1) {
                            pixel->SetEtiquette(eg);
                        }
                        //le voisin gauche n'a pas d'étiquette
                        else if (eg == -1) {
                            pixel->SetEtiquette(eh);
                        } 
                        //les étiquettes existent et sont différentes
                        else {
                            pixel->SetEtiquette(min(eh,eg));
                            //TODO equiv
                        }
                    }

                }//Si le pixel a au moins un voisin null
                else {
                    //les deux voisins sont null
                    if (!vh->isNotNull() && !vg->isNotNull()) {
                        pixel->SetEtiquette(currentEtiq++);
                    }//un des deux voisins est null
                    else {
                        CustomPixel* voisin;
                        if (vh->isNotNull()) {
                            voisin = vh;
                        } else {
                            voisin = vg;
                        }
                        
                        //Verification de l'étiquette du voisin
                        eh = voisin->GetEtiquette();
                        if (eh >= 0){
                            pixel->SetEtiquette(eh);
                        } else {
                            pixel->SetEtiquette(currentEtiq++);
                        }
                    }
                }

            }
        }
    }
    cout<<"algoEtiquetage ends, currentEtiq = "<<currentEtiq<<endl;
    return pixels;
}

IplImage* getEtiquetage() {
    cout << "getEtiq begins" << endl;
    CustomPixel*** pixels = CustomPixel::imageToCustomPixelArray(imageBase);
    pixels = algoEtiquetage(pixels);
    //IplImage * img = CustomPixel::CustomPixelArrayToImage(pixels, imageBase->height, imageBase->width);
    IplImage * img = NULL;
    cout << "getEtiq ends" << endl;
    return img;
}
