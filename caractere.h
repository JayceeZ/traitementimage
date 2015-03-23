#ifndef CARACTERE_H
#define CARACTERE_H

#include <stdlib.h>
#include <vector>

class Caractere {
private:
      char caractere;
      std::vector<std::vector<int> > matrice;

public:
      Caractere(char caractere, std::vector<std::vector<int> > matrice) {
            this->caractere = caractere;
            this->matrice = matrice;
      }

      char getCaractere() {
            return caractere;
      }

      std::vector<std::vector<int> > getMatrice() {
            return matrice;
      }
};

#endif // CARACTERE_H
