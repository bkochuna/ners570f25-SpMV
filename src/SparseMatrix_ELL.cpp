#include "SparseMatrix_ELL.hpp"

#include <iostream>
#include <stddef.h>

#include "SparseMatrix_CSR.hpp" // To help build the ELL from CSR
#include "SparseMatrix_ELL.hpp"

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

// Assemble 
template <class fp_type> 
void SparseMatrix_ELL<fp_type>::assembleStorage( 
    const std::vector<size_t>& i_idx,
    const std::vector<size_t>& j_idx,
    const std::vector<fp_type>& vals,
    size_t nrows,
    size_t ncols){

  // Use CSR class assemble as interface to get rowPtrs, colIdx and values to build ELL matrix
  // Example usage: SparseMatrix_ELL<double> A; A.assembleStorage(i, j, v, 3, 3);

  // Create a temporary CSR matrix
  SparseMatrix_CSR csr;
  csr.assemble(nrows, ncols, val); // TO CONFIRM: PARAMETERS IN CSR ASSAMBLE FUNCTION

  // Use CSR to build the ELL storage

  _nrows = csr.rows(); // TO CONFIRM: PARAMETERS IN CSR CLASS
  _ncols = csr.cols(); // TO CONFIRM: PARAMETERS IN CSR CLASS

  const auto& rowPtr = csr.rowPtr(); // TO CONFIRM: PARAMETERS IN CSR CLASS
  const auto& csrColIdx = csr.colIdx(); // TO CONFIRM: PARAMETERS IN CSR CLASS
  const auto& csrVals = csr.values(); // TO CONFIRM: PARAMETERS IN CSR CLASS

  // Find the max number of nonzeros in any row
  _maxEntriesPerRow = 0;
  for (size_t i = 0; i < _nrows; ++i) {
        size_t rowLen = rowPtr[i + 1] - rowPtr[i];
        _maxEntriesPerRow = std::max(_maxEntriesPerRow, rowLen);
  }
  
  //// Resize internal arrays
  _colIdx.assign(_nrows * _maxEntriesPerRow, std::numeric_limits<size_t>::max());
  _values.assign(_nrows * _maxEntriesPerRow, fp_type(0));

  // Fill them row by row
for (size_t row = 0; row < _nrows; ++row) {
        size_t start = rowPtr[row];
        size_t end   = rowPtr[row + 1];
        for (size_t k = 0; k < end - start; ++k) {
            // ELL column-major layout: each "column" corresponds to the k-th nonzero of each row
            size_t idx = k * _nrows + row;
            _colIdx[idx] = csrColIdx[start + k];
            _values[idx] = csrVals[start + k];
        }
    }
}



}



} // namespace SpMV
// Need to declare the concrete templates within the library for
// use in code that links to libspmv
template class SpMV::SparseMatrix_ELL<float>;
template class SpMV::SparseMatrix_ELL<double>;
