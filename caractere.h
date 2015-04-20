#ifndef CARACTERE_H
#define CARACTERE_H

#include <stdlib.h>
#include <vector>

class Caractere {
private:
      int caractere;
      std::vector<std::vector<int> > matrice;

public:
      Caractere(int caractere, std::vector<std::vector<int> > matrice) {
            this->caractere = caractere;
            this->matrice = matrice;
      }

      int getCaractere() {
            return caractere;
      }

      std::vector<std::vector<int> > getMatrice() {
            return matrice;
      }
};

#endif // CARACTERE_H
