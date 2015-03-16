#include "headers/CImg.h"
#include <iostream>
#include <iomanip>
#include <vector>

using namespace cimg_library;
using namespace std;

#define GRID_SIZE 6

CImg<unsigned char> blackWhite(CImg<unsigned char> src);
vector<vector<int> > matrixBW(CImg<unsigned char> image, int x1, int y1, int x2, int y2);
void drawGrid(CImg<unsigned char>* image, int x1, int y1, int x2, int y2);

int main(int argc, const char* argv[]) {
    // Test de blackwhite
    CImg<unsigned char> src("imgs/couleur.jpg");

	CImg<unsigned char> blackwhite = blackWhite(src);
    (blackwhite).display("Black & White");

    // Test de la méthode de récupération de caractère par contour

    // TODO: intégrer le code d'Alexandre Tissière


    // Test de la méthode de grille
    CImg<unsigned char> lettera("imgs/lettera.jpg");

    vector<vector<int> > matrice(GRID_SIZE*GRID_SIZE);
    //matrice = matrixBW(lettera, 100, 100, 310, 305);
    //drawGrid(&lettera, 100, 100, 310, 305);
    matrice = matrixBW(blackwhite, 170, 69, 183, 84);
    drawGrid(&blackwhite, 170, 69, 183, 84);

    for(int j=0; j < GRID_SIZE; j++) {
        for(int i=0; i < GRID_SIZE; i++) {
            cout << matrice[i][j];
        }
        cout << endl;
    }

    CImgDisplay main_disp(blackwhite, "Input Image");

    while (!main_disp.is_closed() && !main_disp.is_keyESC() && !main_disp.is_keyQ()) {
        cimg::wait(20);
    }

    return 0;
}

CImg<unsigned char> blackWhite(CImg<unsigned char> src)
{
    int width = src.width();
	int height = src.height();
	int depth = src.depth();
	int total = 0;

	//New grayscale images.
	CImg<unsigned char> gray1(width,height,depth,1);
	CImg<unsigned char> blackwhite(width,height,depth,1);

	unsigned char r,g,b;
	unsigned char gr1 = 0;

	/* Convert RGB image to grayscale image */
    cimg_forXY(src,i,j) {

			//Return a pointer to a located pixel value.
			r = src(i,j,0,0); // First channel RED
			g = src(i,j,0,1); // Second channel GREEN
			b = src(i,j,0,2); // Third channel BLUE

			//PAL and NTSC
			//Y = 0.299*R + 0.587*G + 0.114*B
			gr1 = round(0.299*((double)r) + 0.587*((double)g) + 0.114*((double)b));
			gray1(i,j,0,0) = gr1;
			total+=gr1;
	}
	int median = gray1.median();
	int variance = gray1.variance();
	int tab[16];
	for (int i =0; i<16; i++){
            tab[i]=0;
    }

    cimg_forXY(gray1,i,j) {
            gr1 = gray1(i,j,0,0);
            tab[gr1/16]++;
            if (gr1<median-sqrt(variance)){
                blackwhite(i,j,0,0) = 0;
			} else {
			    blackwhite(i,j,0,0) = 255;
			}
    }
    // cout << "Histogramme :" << endl;
    for (int i =0; i<16; i++){
        //cout << "entre " << i*16 << " et " << i*16+15 << ": " << tab[i] << endl;
    }
    return blackwhite;
}

void drawGrid(CImg<unsigned char>* image, int x1, int y1, int x2, int y2) {
    unsigned char black[] = { 0,0,0 };

    int pixelsColumns = (x2-x1)/GRID_SIZE;
    int pixelsLines = (y2-y1)/(GRID_SIZE-1);

    for(int j = 0; j < GRID_SIZE; j++) { // pour chaque ligne de la grille
        for(int i = 0; i < GRID_SIZE; i++) { // Pour chaque cases d'une ligne de la grille
            (*image).draw_rectangle(x1+pixelsColumns*i,y1+j*pixelsLines,x1+(i+1)*pixelsColumns,y1+(j+1)*pixelsLines,black,1,~0U);
        }
    }
}

// Précondition: (x1,y1) plus grand que (x2, y2)
vector<vector<int> > matrixBW(CImg<unsigned char> image, int x1, int y1, int x2, int y2) {
    vector<vector<int> > matrice(GRID_SIZE*GRID_SIZE);

    int pixelsColumns = (x2-x1)/GRID_SIZE;
    int pixelsLines = (y2-y1)/(GRID_SIZE-1);

    int nbblanc = 0;
    int nbnoir = 0;
    int valeur;

    for(int j = 0; j < GRID_SIZE; j++) { // pour chaque ligne de la grille
        for(int i = 0; i < GRID_SIZE; i++) { // Pour chaque cases d'une ligne de la grille
            nbblanc = 0; nbnoir = 0;
            // on réalise le parcours des pixels dont on compte la quantité de noir ou de blanc
            for(int runY = j*pixelsLines + y1; (runY - y1) < (j+1)*pixelsLines; runY++) {
                for(int runX = i*pixelsColumns + x1; (runX - x1) < (i+1)*pixelsColumns; runX++) {
                    if(image(runX,runY,0,0) == 255) // blanc
                        nbblanc++;
                    else
                        nbnoir++;
                }
            }
            if (nbblanc > nbnoir)
                valeur = 1; // majorité blanc
            else
                valeur = 0; // majorité noir
            matrice[i].push_back(valeur);
        }
    }

    return matrice;
}
