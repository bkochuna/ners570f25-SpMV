#include "SparseMatrix_ELL.hpp"

#include <iostream>
#include <stdexcept>
#include <stddef.h>

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
} // namespace SpMV
// Need to declare the concrete templates within the library for
// use in code that links to libspmv
template class SpMV::SparseMatrix_ELL<float>;
template class SpMV::SparseMatrix_ELL<double>;
