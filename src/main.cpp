#include "CImg.h"
#include <iostream>
#include <iomanip>
#include <vector>

using namespace cimg_library;
using namespace std;

#define GRID_SIZE_X 9

char reconnaitre(CImg<unsigned char> image, int angles[][4]);

int main(int argc, const char* argv[]) {
  CImg<unsigned char> image = CImg<>("imgs/letter_reduced.jpg");

  //image.load(file_i);

  // values depending on the test image choosen
  int angles[][4] { { 0, 0 }, { 0, 882 }, { 847, 0 }, { 847, 882 } };
  char caractere = reconnaitre(image, angles);



  CImgDisplay main_disp(image, "Input Image");

  while (!main_disp.is_closed() && !main_disp.is_keyESC() && !main_disp.is_keyQ()) {
    cimg::wait(20);
  }

    vector<unsigned int> vecteur;

  //image.save("imgs/test.png");
  return 0;
}

char reconnaitre(CImg<unsigned char> image, int angles[][4]) {
    unsigned char* data = image.data();

    for(int x=0; x<GRID_SIZE_X; x++) {
        cout << image.pixel_type( << endl;
    }

    return 0;
}
