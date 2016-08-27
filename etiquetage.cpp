#include <cstdlib>
#include <opencv2/core/types_c.h>
#include <opencv2/core/core_c.h>
#include <opencv2/highgui/highgui_c.h>
#include <iostream>


const char * const W_BASE = "Image initiale";
const char * const W_ETIQ = "Image etiquetée";

using namespace std;

IplImage * imageBase,
        * imageEtiq;

bool isValidCommand(int nbArgs, char** args);

int main(int argc, char** argv) {
    //verification des arguments du proramme
    if (!isValidCommand(argc, argv)) return EXIT_FAILURE;
    
    //initialisation des fenetres
    cvNamedWindow(W_BASE);
    cvNamedWindow(W_ETIQ);
    
    //etiquetage
    //TODO
    
    //Affichage des images
    cvShowImage(W_BASE, imageBase);
    cvShowImage(W_ETIQ, imageEtiq);
    
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
bool isValidCommand(int nbArgs, char** args){
    //Verification du nombre d'arguments
    if (nbArgs != 3){
        cerr << "Utilisation: \n\t etiquetage <nom_fichier_image>"<<endl;
            return false;
    }
    
    //Chargement des images
    IplImage * tmp = cvLoadImage(args[1], CV_LOAD_IMAGE_GRAYSCALE);
    if (tmp == 0){
        cerr << "Impossible de lire : " << args[1] << "\n";
            return false;
    }
    imageBase = cvCloneImage(tmp);
}