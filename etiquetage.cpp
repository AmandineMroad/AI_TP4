#include <cstdlib>
#include <opencv2/core/types_c.h>
#include <opencv2/core/core_c.h>
#include <opencv2/highgui/highgui_c.h>
#include <iostream>
#include <algorithm>
#include <opencv2/imgproc/imgproc.hpp>


#include "CustomPixel.h"

#define ARRAY_SIZE(array) (sizeof((array))/sizeof((array[0])))

const char * const W_BASE = "Image initiale";
const char * const W_ETIQ = "Image etiquetée";

using namespace std;
using namespace cv;

const IplImage * imageBase;

bool isValidCommand(int nbArgs, char** args);
inline IplImage* getEtiquetage();
IplImage* algoEtiquetage(const IplImage* image);
void addEquivalence(int e1, int e2);

int main(int argc, char** argv) {
    //verification des arguments du proramme
    if (!isValidCommand(argc, argv)) return EXIT_FAILURE;

    //initialisation des fenetres
    cvNamedWindow(W_BASE);
    cvNamedWindow(W_ETIQ);

    //Affichage des images
    cvShowImage(W_BASE, imageBase);
    cvShowImage(W_ETIQ, algoEtiquetage(imageBase));

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

IplImage* algoEtiquetage(const IplImage* image) {
    const int nbLignes = image->height;
    const int nbCol = image->width;
    IplImage* imgEtiq = cvCreateImage(cvSize(nbCol, nbLignes), IPL_DEPTH_8U, 1);

    //Création de la matrice d'étiquettes
    int ** mat = new int*[nbLignes];

    int i, j, e, eh, eg;
    int currentEtiq = 0;

    //Parcours de l'image
    for (i = 0; i < nbLignes; i++) {
        int offset = i*nbCol; //TODO del
        mat[i] = new int[nbCol];
        for (j = 0; j < nbCol; j++) {
            e = -1;

            //Si le pixel n'appartient pas au fond (ici <230)
            if (cvGet2D(image, i, j).val[0] < 230) {
                cout << "pixel>230";
                imgEtiq->imageData[j + offset] = 255; //TODO del

                if (i == 0 && j == 0) { //premier pixel
                    cout << "first" << endl;
                    e = currentEtiq++;
                } else if (i == 0) { //première ligne (pas de voisin haut)
                    cout << "1st line" << endl;
                    eg = mat[i][j - 1];
                    if (eg != -1) { //le voisin gauche porte une etiquette
                        e = eg;
                        cout << "etiq à gauche" << endl;
                    } else {
                        cout << "current + " << endl;
                        e = currentEtiq++;
                    }
                } else if (j == 0) { //première colonne (pas de voisin gauche)
                    cout << "1st col" << endl;
                    eh = mat[i - 1][j];
                    if (eh != -1) {
                        cout << "etiq en haut" << endl;
                        e = eh;
                    } else {
                        cout << "current + " << endl;
                        e = currentEtiq++;
                    }
                } else { //le pixel a deux voisins
                    eh = mat[i - 1][j];
                    eg = mat[i][j - 1];

                    if (eh == eg) {
                        //Les deux voisins ont la meme etiquette
                        if (eh == -1) {
                            e = currentEtiq++;
                            cout << "current + " << endl;
                        } else {
                            e = eh;
                            cout << "same etiq " << endl;
                        }

                    } else {
                        //les voisins ont des etiquettes differentes
                         if (eh == -1 || eg == -1) {
                            //Un des voisins n'a pas d'étiquette
                            e = max(eh, eg);
                            cout << " un vois sans e" << endl;
                        } else {
                            e = min(eh, eg);
                            cout << "equiv ! " << endl;
                            addEquivalence(eh, eg);
                        }
                    }
                }
            } else {
                imgEtiq->imageData[j + offset] = 0;
            }
            mat[i][j] = e;
        }
    }
    cout << "fin etiquetage nb etiq = " << currentEtiq << endl;
    return imgEtiq;
}

//Map d'équivalence
map<int, int* > equivMap;

void addEquivalence(int e1, int e2) {
    cout << "call: addEquivalence(" << e1 << "," << e2 << ")" << endl;

    // e1 doit être inférieur à e2
    if (e1 > e2) swap(e1, e2);

    int* list_tmp = equivMap[e1];
    if (list_tmp == NULL) {//map.count()
        //la paire n'existe pas
        list_tmp = new int[2];
        list_tmp[0] = e2;
        list_tmp[1] = 0;
        equivMap.insert(make_pair(e1, list_tmp));
    } else {
        //Calcul de la taille du tableau d'équivalence de e1
        int cpt = 0;
        while (list_tmp[cpt++] != 0);
        const int size = cpt + 1;

        int *newList = new int[size];
        //copie de la liste precedente
        for (int i = 0; i < size - 2; i++) {
            if (newList[i] == e2) {
                //e2 est deja dans la liste d'equivalence de e1
                delete[] newList;
                return;
            }
            newList[i] = list_tmp[i];

        }
        //insertion du dernier element
        newList[size - 2] = e2;
        newList[size - 1] = 0;

        //stockage de la nouvelle list et destruction de l'ancienne
        equivMap[e1] = newList;
        delete[] list_tmp;
    }
}