#include "headers/CImg.h"
#include "caractere.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>

using namespace cimg_library;
using namespace std;

#define GRID_SIZE 32.0

vector<vector<int> > contour;
CImg<unsigned char> image;

vector<vector<int> > fusionRectangle(vector<vector<int> > rectangles);
vector<vector<int> > triRectangles(vector<vector<int> > rectangles);
float compareMatrix(vector<vector<int> > mat1, vector<vector<int> > mat2);
string detectFromImage(string path);
void loadSamplePolice();
void addContour(int x, int y);
vector<vector<int> > detection_rectangles();

CImg<unsigned char> blackWhite(CImg<unsigned char> src);
vector<vector<int> > matrixBW(CImg<unsigned char> image, int x1, int y1, int x2, int y2);
void drawGrid(CImg<unsigned char>* image, int x1, int y1, int x2, int y2);
void genererMatrices(char* caracteres);

vector<Caractere> objetsCaractere;

int main(int argc, const char* argv[]) {
    loadSamplePolice();
    string test = detectFromImage("imgs/test.jpg");
    cout << "test : " << test << endl;

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

    float pixelsColumns = (x2-x1)/GRID_SIZE;
    float pixelsLines = (y2-y1)/GRID_SIZE;

    for(int j = 0; j <= GRID_SIZE; j++) { // pour chaque ligne de la grille
        for(int i = 0; i < GRID_SIZE; i++) { // Pour chaque cases d'une ligne de la grille
            (*image).draw_rectangle(x1+pixelsColumns*i,y1+j*pixelsLines,x1+(i+1)*pixelsColumns,y1+(j+1)*pixelsLines,black,1,~0U);
        }
    }
}

// Précondition: (x1,y1) plus grand que (x2, y2)
vector<vector<int> > matrixBW(CImg<unsigned char> image, int x1, int y1, int x2, int y2) {
    vector<vector<int> > matrice(GRID_SIZE*GRID_SIZE);

    CImg<unsigned char> image_trunc = image.get_crop(x1,y1,x2,y2);

    //cout << pixelsColumns << " " << pixelsLines << endl;

    int nbblanc = 0;
    int nbnoir = 0;
    int valeur;

    while(image_trunc.width() < GRID_SIZE || image_trunc.height() < GRID_SIZE) {
        image_trunc.resize_doubleXY();
    }

    float pixelsColumns = image_trunc.width()/GRID_SIZE;
    float pixelsLines = image_trunc.height()/GRID_SIZE;

    for(int j = 0; j <= GRID_SIZE; j++) { // pour chaque ligne de la grille
        for(int i = 0; i < GRID_SIZE; i++) { // Pour chaque cases d'une ligne de la grille
            nbblanc = 0; nbnoir = 0;
            // on réalise le parcours des pixels dont on compte la quantité de noir ou de blanc
            for(int runY = j*pixelsLines; runY < (j+1)*pixelsLines; runY++) {
                for(int runX = i*pixelsColumns; runX < (i+1)*pixelsColumns; runX++) {
                    if(image_trunc(runX,runY,0,0) == 255) // blanc
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

void genererMatrices(char* caracteres) {
    for(int k=0; k < strlen(caracteres); k++) {
        char caractere = caracteres[k];
        stringstream concatenation;
        concatenation << "imgs/" << caractere << ".jpg";
        string fichier = concatenation.str();
        std::cout << "creation de " << caractere << endl;

        CImg<unsigned int> lettre(fichier.data());

        Caractere objet(caractere, matrixBW(lettre, 0, 0, lettre.width(), lettre.height()));
        objetsCaractere.push_back(objet);
    }

    return;
}

vector<vector<int> > detection_rectangles(){
    vector<vector<int> > rectangles;
    int x1, x2, y1, y2;
    cimg_forXY(image,i,j) {
        if(image(i,j,0,0) == 0){
            contour.clear();
            addContour(i,j);
            //(image).display("test");
            x1 = i; x2 = i; y1 = j; y2 = j;
            for(int a = 0; a < contour.size() ; a++){
                if(contour[a][0] < x1)
                    x1 = contour[a][0];
                if(contour[a][0] > x2)
                    x2 = contour[a][0];
                if(contour[a][1] < y1)
                    y1 = contour[a][1];
                if(contour[a][1] > y2)
                    y2 = contour[a][1];
            }
            rectangles.push_back(vector<int>(0));
            rectangles[rectangles.size()-1].push_back(x1);
            rectangles[rectangles.size()-1].push_back(y1);
            rectangles[rectangles.size()-1].push_back(x2);
            rectangles[rectangles.size()-1].push_back(y2);
            //i = x2 + 1;
            //j = ( y1 + y2 ) / 2;
        }

    }
    return rectangles;
}

void addContour(int x, int y){
    contour.push_back(vector<int>(2));
    contour[contour.size()-1][0] = x;
    contour[contour.size()-1][1] = y;
    image(x,y,0,0) = 255;
    if(x+1 < image.width() && image(x+1,y,0,0) == 0){
        addContour(x+1,y);}
    if(x-1 > 0 && image(x-1,y,0,0) == 0 ){
        addContour(x-1,y);}
    if(y+1 < image.height() && image(x,y+1,0,0) == 0 ){
        addContour(x,y+1);}
    if(y-1 > 0 && image(x,y-1,0,0) == 0 ){
        addContour(x,y-1);}

}

void loadSamplePolice(){
    CImg<unsigned char> src("imgs/arialblack.jpg");

	image = blackWhite(src);
	CImg<unsigned char> imgcopy(image);
	vector<vector<int> > caracteres = detection_rectangles();
	caracteres = triRectangles(caracteres);

	char* alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890abcdeéèêfghijklmnopqrstuvwxyz?.!,:/";
	for(int i = 0; i < caracteres.size(); i++){

        vector<vector<int> > mcaractere = matrixBW(imgcopy,caracteres[i][0],caracteres[i][1],caracteres[i][2],caracteres[i][3]);
        Caractere caractere(alphabet[i],mcaractere);
        objetsCaractere.push_back(caractere);

	}
}

string detectFromImage(string path){
    string result = "";
    CImg<unsigned char> src(path.c_str());
    image = blackWhite(src);
	CImg<unsigned char> imgcopy(image);

	vector<vector<int> > caracteres = detection_rectangles();
	caracteres = triRectangles(caracteres);
	caracteres = fusionRectangle(caracteres);

	for(int i = 0; i < caracteres.size(); i++){
        if(i!= 0){
            if(caracteres[i][1] >= caracteres[i-1][3])
                result += '\n';
            else if (caracteres[i][0]-caracteres[i-1][2] > (caracteres[i][3] - caracteres[i][1])/2  )
                result += ' ';
        }
        char caractere = '?';
        vector<vector<int> > mcaractere = matrixBW(imgcopy,caracteres[i][0],caracteres[i][1],caracteres[i][2],caracteres[i][3]);

        for(int j=0; j < GRID_SIZE; j++) {
            for(int i=0; i < GRID_SIZE; i++) {
                std::cout << mcaractere[i][j];
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;

        int best = 0;
        for(int a = 0; a < objetsCaractere.size(); a++){
            float correspondance = compareMatrix(mcaractere,objetsCaractere[a].getMatrice());

            if(correspondance > 75 && correspondance >= best){
                best = correspondance;
                caractere = objetsCaractere[a].getCaractere();
            }

        }
        result += caractere;
	}
	return result;
}

float compareMatrix(vector<vector<int> > mat1, vector<vector<int> > mat2){
    float corr = GRID_SIZE*GRID_SIZE;
    for(int x = 0; x < GRID_SIZE ; x++){
        for(int y = 0; y < GRID_SIZE; y++){
            if(mat1[x][y] != mat2[x][y])
                corr--;
        }
    }
    corr = corr * 100 / (GRID_SIZE*GRID_SIZE);
    return corr;
}

vector<vector<int> > triRectangles(vector<vector<int> > rectangles){
    vector<vector<int> > tri;
    for(int i = 0; i < rectangles.size(); i++){
        vector<int> rectangle;
        rectangle.push_back(rectangles[i][0]);
        rectangle.push_back(rectangles[i][1]);
        rectangle.push_back(rectangles[i][2]);
        rectangle.push_back(rectangles[i][3]);
        if(tri.empty())
            tri.push_back(rectangle);
        else{
        int j,index = tri.size();
            for(j = tri.size()-1; j >= 0; j--){
                if(rectangle[3] < tri[j][1] || (rectangle[1] <= tri[j][3] && rectangle[2] <= tri[j][0])){
                    index = j;
                }
            }
            if(index == tri.size()){
                tri.push_back(rectangle);
            }
            else{
                tri.insert(tri.begin()+index, rectangle);
            }
        }
    }
    return tri;
}

vector<vector<int> > fusionRectangle(vector<vector<int> > rectangles){
    vector<vector<int> > fusion;
    int y1 = rectangles[0][1] , y2 = rectangles[0][3], i;
    for(i = 1; i < rectangles.size(); i++){
        int nx1 = rectangles[i-1][0], ny1 = rectangles[i-1][1], nx2 = rectangles[i-1][2], ny2 = rectangles[i-1][3];
        if(rectangles[i][1] > y2 ){
            if(rectangles[i][3] - rectangles[i][1] < y2 - y1)
                y2 = ( y2 - y1 ) + rectangles[i][1];
            else
                y2 = rectangles[i][3];
            y1 = rectangles[i][1];
        }
        else{
            if( rectangles[i][1] < y1) y1 = rectangles[i][1];
            if( rectangles[i][3] > y2) y2 = rectangles[i][3];
            if( !(rectangles[i][0] > rectangles[i-1][2]) && !(rectangles[i][2] < rectangles[i-1][0])){
                if(rectangles[i][0] < nx1) nx1 = rectangles[i][0];
                if(rectangles[i][1] < ny1) ny1 = rectangles[i][1];
                if(rectangles[i][2] > nx2) nx2 = rectangles[i][2];
                if(rectangles[i][3] > ny2) ny2 = rectangles[i][3];
                i++;
            }
        }
        vector<int> rectangle;
        rectangle.push_back(nx1);
        rectangle.push_back(ny1);
        rectangle.push_back(nx2);
        rectangle.push_back(ny2);
        fusion.push_back(rectangle);
    }
    if(i == rectangles.size()){
        vector<int> rectangle;
        rectangle.push_back(rectangles[i-1][0]);
        rectangle.push_back(rectangles[i-1][1]);
        rectangle.push_back(rectangles[i-1][2]);
        rectangle.push_back(rectangles[i-1][3]);
        fusion.push_back(rectangle);
    }
    return fusion;
}













