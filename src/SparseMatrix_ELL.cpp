#include "SparseMatrix_ELL.hpp"

#include <iostream>
#include <stddef.h>
#include <iomanip>
#include <cmath>
#include <vector>

using namespace std;

namespace SpMV {

// Constructor
template <class fp_type>
SparseMatrix_ELL<fp_type>::SparseMatrix_ELL(const size_t nrows,
                                            const size_t ncols)
    : SparseMatrix<fp_type>::SparseMatrix(nrows, ncols) {
  cout << "Hello from SparseMatrix_ELL Constructor" << endl;
}

// Destructor
template <class fp_type> SparseMatrix_ELL<fp_type>::~SparseMatrix_ELL() {
  cout << "Goodbye from SparseMatrix_ELL Destructor" << endl;
}

// Assemble placeholder
template <class fp_type> void SparseMatrix_ELL<fp_type>::assemble() {
  cout << "Hello from SparseMatrix_ELL assemble" << endl;
}

// View Method
template <class fp_type> void SparseMatrix_ELL<fp_type>::viewMat() {
  const int nrows = this->_nrows;
  const int ncols = this->_ncols;
  const auto& maxNnz = this->getmaxNNZPerRow();
  const auto& values = this->getValues();
  const auto& colInd = this->getColInd();
  std::vector<fp_type> mat(nrows * ncols, 0.0);
  int maxDig = 1;

  for (int i=0;i<nrows;i++){ // restructure as an ij grid for printing
      for (int j=0;j<maxNnz;j++) {
          int col = colInd[i*maxNnz+j];
          if (col != -1) {
            mat[i*ncols+col] = values[i*maxNnz+j];
            if ((floor(log10(abs(values[i*maxNnz+j]))))+1> (log10(maxDig))) {
              maxDig = static_cast<int>(floor(log10(abs(values[i*maxNnz+j])))+1);
            }
          }
      }
  }
  cout << fixed << setprecision(8); // will print only 8 decimal places for viewing, but can change as desired
  int printWidth = (2+maxDig) + (8); // prints separator space before longest whole number plus decimal dot, plus number of decimals (change corresponding to value above)
  cout << "A=[ \n";
  for (int i=0;i<nrows;i++) {
    for (int j=0;j<ncols;j++) {
        cout << setw(printWidth) << mat[i*ncols+j];
    }
    if (i==nrows-1){
        cout << "]" << endl;
        return;
    }
    else {
        cout << "\n";
    }
  }
}
} // namespace SpMV
// Need to declare the concrete templates within the library for
// use in code that links to libspmv
template class SpMV::SparseMatrix_ELL<float>;
template class SpMV::SparseMatrix_ELL<double>;
