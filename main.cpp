#include "headers/CImg.h"
#include <iostream>
#include <iomanip>
#include <vector>

using namespace cimg_library;
using namespace std;

#define GRID_SIZE 16

vector<vector<int> > matrixBW(CImg<unsigned char> image, int x1, int y1, int x2, int y2);
void drawGrid(CImg<unsigned char>* image, int x1, int y1, int x2, int y2);

int main(int argc, const char* argv[]) {
  CImg<unsigned char> image = CImg<>("imgs/lettera.jpg");

  //image.load(file_i);

  vector<vector<int> > matrice(GRID_SIZE*GRID_SIZE);
  // values depending on the test image choosen
  matrice = matrixBW(image, 100, 100, 310, 305);
  drawGrid(&image, 100, 100, 310, 305);

  for(int j=0; j < GRID_SIZE; j++) {
    for(int i=0; i < GRID_SIZE; i++) {
        cout << matrice[i][j];
    }
    cout << endl;
  }

  //(image).display("prout");
  CImgDisplay main_disp(image, "Input Image");

  while (!main_disp.is_closed() && !main_disp.is_keyESC() && !main_disp.is_keyQ()) {
    cimg::wait(20);
  }

  //image.save("imgs/test.png");
  return 0;
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

// Précondition: x2 plus grand que x1
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
