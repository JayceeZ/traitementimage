#ifndef CARACTERE_H
#define CARACTERE_H

#include <vector>

class Caractere {
private:
      char caractere;
      vector<vector<int> > matrice;

public:
      Caractere(char caractere, vector<vector<int> > matrice) {
            this->caractere = caractere;
            this->matrice = matrice;
      }

      char getCaractere() {
            return caractere;
      }

      vector<vector<int> > getMatrice() {
            return matrice;
      }
};

#endif // CARACTERE_H
