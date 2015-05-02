#ifndef CARACTERE_H
#define CARACTERE_H

#include <stdlib.h>
#include <vector>

class Caractere {
private:
      int caractere;
      std::vector<std::vector<int> > matrice;
      float proportion;// Hauteur sur largeur

public:
      Caractere(int caractere, std::vector<std::vector<int> > matrice, int width, int height) {
            this->caractere = caractere;
            this->matrice = matrice;
            this->proportion = (float)height/(float)width;
      }

      int getCaractere() {
            return caractere;
      }

      std::vector<std::vector<int> > getMatrice() {
            return matrice;
      }

      float getProportion(){
            return proportion;
      }
};

#endif // CARACTERE_H
