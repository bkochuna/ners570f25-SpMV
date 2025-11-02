#include "SparseMatrix_ELL.hpp"

#include <iostream>
#include <stdexcept>
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
  
}

// Destructor
template <class fp_type> SparseMatrix_ELL<fp_type>::~SparseMatrix_ELL() {
  
}

// Assemble placeholder
template <class fp_type> void SparseMatrix_ELL<fp_type>::assemble() {
  
}

// Perform a matrix-vector multiply using the stored ELL data layout.
template <class fp_type>
std::vector<fp_type>
SparseMatrix_ELL<fp_type>::matvec(const std::vector<fp_type> &x) const {
  if (this->_state != MatrixState::assembled) {
    throw runtime_error("SparseMatrix_ELL::matvec requires an assembled matrix");
  }
  if (x.size() != this->_ncols) {
    throw runtime_error("SparseMatrix_ELL::matvec received vector with incompatible length");
  }

  std::vector<fp_type> y;
  y.reserve(this->_nrows);

  if (this->_nrows == 0 || _maxEntriesPerRow == 0) {
    for (std::size_t row = 0; row < this->_nrows; ++row) y.push_back(fp_type{});
    return y;
  }
  if (_values.size() != this->_nrows * _maxEntriesPerRow ||
      _colIdx.size() != _values.size()) {
    throw runtime_error("SparseMatrix_ELL::matvec storage is inconsistent");
  }

  for (std::size_t row = 0; row < this->_nrows; ++row) {
    const std::size_t base = row * _maxEntriesPerRow;
    fp_type sum{}; // accumulate in a scalar
    for (std::size_t idx = 0; idx < _maxEntriesPerRow; ++idx) {
      const std::size_t col = _colIdx[base + idx];
      if (col == this->_ncols) {
        continue; // padded slot
      }
      if (col > this->_ncols) {
        throw runtime_error("SparseMatrix_ELL::matvec encountered column index out of bounds");
      }
      sum += _values[base + idx] * x[col];
    }
    y.push_back(sum); // assign once per row
  }

  return y;
}

// View Method
template <class fp_type> void SparseMatrix_ELL<fp_type>::viewMat() {
  const int nrows = this->_nrows;
  const int ncols = this->_ncols;
  const auto& maxNnz = this->getmaxNNZPerRow();
  const auto& values = this->getValues();
  const auto& colInd = this->getColInd();
  std::vector<fp_type> mat(nrows * ncols, 0.0); // creating DEN format matrix for printing
  int maxDig = 1; // tracking and updating the max number of whole digits in matrix values

  // Converting ELL format to DEN format for printing to ensure all zeros and
  // non-zero values are printed in the correct ij location
  for (int i=0;i<nrows;i++){ // restructure as an ij grid for printing
      for (int j=0;j<maxNnz;j++) {
          int col = colInd[i*maxNnz+j];
          if (col != -1) {
            mat[i*ncols+col] = values[i*maxNnz+j]; // fill non-zero numbers into preset zero vector
            if ((floor(log10(abs(values[i*maxNnz+j]))))+1> (log10(maxDig))) {
              maxDig = static_cast<int>(floor(log10(abs(values[i*maxNnz+j])))+1); // updating max number of digits in whole number representation for printing purposes
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
} 

template class SpMV::SparseMatrix_ELL<float>;
template class SpMV::SparseMatrix_ELL<double>;
