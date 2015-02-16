#include "CImg.h"
#include <iostream>
#include <iomanip>

using namespace cimg_library;
using namespace std;

int main(int argc, const char* argv[]) {
  CImg<unsigned char> image = CImg<>("imgs/test.png");

  //image.load(file_i);
  image.projections2d(5, 10, 1);
  image.blur(1);

  CImgDisplay main_disp(image, "Input Image");

  while (!main_disp.is_closed() && !main_disp.is_keyESC() && !main_disp.is_keyQ()) {
    cimg::wait(20);
  }



  //image.save("imgs/test.png");
  return 0;
}
