#include "SparseMatrix_ELL.hpp"

#include <iostream>
#include <stddef.h>

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
  // Unsure how all these functions will be named: getELLMatrix(), get_values(), getELLMatrix(), get_colIndices()
  auto ELLmat = this->getELLMatrix(); // unsure how this will be stored during implementation
  double* mat = new double[nrows*ncols]();
  int col, maxDig;
  maxDig = 0;
  for (int i=0;i<ELLmat.nrows;i++){ // restructure as an ij grid for printing
      for (int j=0;j<ELLmat._maxNnzPerRow;j++) {
          col = ELLmat.get_colIndices(i*ELLmat._maxNnzPerRow+j);
          mat[i*ELLmat.ncols+col] = ELLmat.get_values(i*ELLmat._maxNnzPerRow+j);
          if ((log10(ELLmat.get_values(i*ELLmat._maxNnzPerRow+j)))> (log10(maxDig))) {
              maxDig = static_cast<int>(log10(ELLmat.get_values(i*ELLmat._maxNnzPerRow+j)));
          }
      }
  }
  maxDig = maxDig+1;
  cout << "A=[";
  int numDig; 
  for (int i=0;i<ELLmat.nrows;i++) {
      for (int j=0;j<ELLmat.ncols;j++) {
          if (ELLmat.get_colIndices(i*ELLmat._maxNnzPerRow+j)  == -1) { // padding value is -1
              numDig = 1;
          }
          else {
              numDig = static_cast<int>(log10(mat[i*ELLmat._maxNnzPerRow+j]))+1;
          }
          
          if (numDig<maxDig){
              for (int x=numDig;x<maxDig;x++){
                  cout << " ";
              }
          }
          if (ELLmat.get_colIndices(i*ELLmat._maxNnzPerRow+j)  == -1) {
              cout << "0.00";
          }
          else {
              cout << fixed << setprecision(2) << mat[i*ELLmat._maxNnzPerRow+j];
          }

          if (i<ELLmat.ncols-1){
              cout << " ";
          }
      }
      if (i==ELLmat.nrows-1){
          cout << "]" << endl;
          return;
      }
      cout << "" << endl;
      cout << "   ";
  }
                    
    }
} // namespace SpMV
// Need to declare the concrete templates within the library for
// use in code that links to libspmv
template class SpMV::SparseMatrix_ELL<float>;
template class SpMV::SparseMatrix_ELL<double>;
