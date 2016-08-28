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
CustomPixel** algoEtiquetage(CustomPixel** pixels);
inline IplImage* getEtiquetage();

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

CustomPixel** algoEtiquetage(CustomPixel** pixels) {
    cout<<"algoEtiq begins"<<endl;
    int nbCol = imageBase->width;
    int nbLigne = imageBase->height;

    int i, j, eh, eg;
    CustomPixel* pixel,
            * vh,
            * vg;
    int numero = 0;

    //TODO delete
    int cptPix=0, cptCont =0;
    
    for (i = 0; i < nbLigne; i++) {
        /***DEBUG */ //TODO Del
         cout<<"ligne "<<i
                <<": pix non blancs = "<<cptPix
                <<" _ contours = "<<cptCont<<endl;
                
                /******/
        for (j = 0; j < nbCol; j++) {
            pixel = &pixels[i][j];
            if (pixel->GetValeur() == 0) {
                cout<<"f, "<<endl;
                pixel->SetValeur(255); //TODO delete
            } //Si le pixel n'est pas de la couleur du fond (blanc))
            else {
                
                /***DEBUG */ //TODO Del
                cout<<"p, "<<endl;
                cptPix++;
                /****/
                vh = pixel->GetVHaut();
                cout<<"getVHaut : "<<pixel->GetVHaut()
                        <<" _ vh : "<<vh<<endl;
                vg = pixel->GetVGauche();
                cout<<"getVG : "<<pixel->GetVGauche()
                        <<" _ vg : "<<vg<<endl;

                //Si le pixel a deux voisins
                if ((vg)->isNotNull() && (vh)->isNotNull()) {
                    cout<<"vg = null : "<<(vg == NULL)
                            << " _ vg* = null : "
                            <<(*vg).isNotNull()
                            <<" / vh = null : "<<(vh == NULL)
                            <<" / vh = isnotnull : "<<vh->isNotNull()
                            << " _ vh* = null : "
                            <<(*vh).isNotNull()
                            <<endl; //TODO del
                    eh = vh->GetEtiquette();
                    
                    eg = vg->GetEtiquette();

                    if (eh == eg) {
                        pixel->SetEtiquette(eh);
                    } else {
                        pixel->SetEtiquette(min(eh, eg));
                        //TODO
                    }

                }//Si le pixel a au moins un voisin null
                else {
                    /***DEBUG */ //TODO Del
                    cout<<"au moins un voisin null"<<endl;
                cptCont++;
                /****/
                    //si le pixel n'a aucun voisin
                    if (!vg->isNotNull() && !vh->isNotNull()) {
                        pixel->SetEtiquette(numero++);
                    } else {
                        CustomPixel* voisin;
                        if (vg->isNotNull()) {
                            voisin = vg;
                        } else {
                            voisin = vh;
                        }
                        eg = voisin->GetEtiquette();
                        if (eg >= 0) {
                            pixel->SetEtiquette(eg);
                        } else {
                            pixel->SetEtiquette(numero++);
                        }
                    }
                //TODO Del
                cout<<"numero = "<<numero<<endl;
                }
            }
        }
    }
    cout<<"FIN ALGO _ nbEtiquette = "<<numero<<endl;
    return pixels;
}

IplImage* getEtiquetage() {
    cout<<"getEtiq begins"<<endl;
    CustomPixel** pixels = CustomPixel::imageToCustomPixelArray(imageBase);
    //pixels = algoEtiquetage(pixels);
    IplImage * img = CustomPixel::CustomPixelArrayToImage(pixels, imageBase->height, imageBase->width);
    cout<<"getEtiq ends"<<endl;
    return img;
}
