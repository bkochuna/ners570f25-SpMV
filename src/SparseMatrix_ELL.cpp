#include "SparseMatrix_ELL.hpp"

#include <stdexcept>

namespace SpMV {

// Constructor: just forward to the base class.
template <class fp_type>
SparseMatrix_ELL<fp_type>::SparseMatrix_ELL(std::size_t nrows, std::size_t ncols)
  : SparseMatrix<fp_type>(nrows, ncols) {}

// Destructor: default.
template <class fp_type>
SparseMatrix_ELL<fp_type>::~SparseMatrix_ELL() = default;

// Assemble placeholder: mark as assembled for now.
// In a full implementation, you would transform base build buffers into ELL storage.
template <class fp_type>
void SparseMatrix_ELL<fp_type>::assemble() {
  // TODO: populate _values, _colIdx, and _maxEntriesPerRow from the base builder.
  this->_state = MatrixState::assembled;
}

// Perform a matrix-vector multiply using the stored ELL data layout.
template <class fp_type>
std::vector<fp_type> SparseMatrix_ELL<fp_type>::matvec(const std::vector<fp_type> &x) const {
  if (this->_state != MatrixState::assembled) {
    throw std::runtime_error("SparseMatrix_ELL::matvec requires an assembled matrix");
  }
  if (x.size() != this->_ncols) {
    throw std::runtime_error("SparseMatrix_ELL::matvec received vector with incompatible length");
  }

  std::vector<fp_type> y;
  y.reserve(this->_nrows);

  if (this->_nrows == 0 || _maxEntriesPerRow == 0) {
    // Return a zero vector of correct length.
    for (std::size_t row = 0; row < this->_nrows; ++row) y.push_back(fp_type{});
    return y;
  }
  if (_values.size() != this->_nrows * _maxEntriesPerRow ||
      _colIdx.size()  != _values.size()) {
    throw std::runtime_error("SparseMatrix_ELL::matvec storage is inconsistent");
  }

  for (std::size_t row = 0; row < this->_nrows; ++row) {
    const std::size_t base = row * _maxEntriesPerRow;
    fp_type sum{}; // accumulate in a scalar
    for (std::size_t idx = 0; idx < _maxEntriesPerRow; ++idx) {
      const std::size_t col = _colIdx[base + idx];
      if (col == this->_ncols) {
        // Padded slot: skip
        continue;
      }
      if (col > this->_ncols) {
        throw std::runtime_error("SparseMatrix_ELL::matvec encountered column index out of bounds");
      }
      sum += _values[base + idx] * x[col];
    }
    y.push_back(sum);
  }
  return y;
}

// Explicit template instantiations to emit symbols in the shared library.
template class SparseMatrix_ELL<float>;
template class SparseMatrix_ELL<double>;

} // namespace SpMV
