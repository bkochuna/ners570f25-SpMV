#include "SparseMatrix_ELL.hpp"

#include <iostream>
#include <stddef.h>

#include "SparseMatrix_CSR.hpp" // To help assamble the ELL from CSR
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
  csr.assemble(i_idx, j_idx, vals, nrows, ncols); // TO CONFIRM: PARAMETERS IN CSR ASSAMBLE FUNCTION

  // Use CSR to build the ELL storage

  // Copy dimensions
  _nrows = csr.rows();  // TO CONFIRM: accessor names
  _ncols = csr.cols();  // TO CONFIRM: accessor names

  // Access CSR storage
  const auto& rowPtr = csr.rowPtr(); // TO CONFIRM: PARAMETERS IN CSR CLASS
  const auto& csrColIdx = csr.colIdx(); // TO CONFIRM: PARAMETERS IN CSR CLASS
  const auto& csrVals = csr.values(); // TO CONFIRM: PARAMETERS IN CSR CLASS

  // Find the max number of nonzeros in any row
  _maxEntriesPerRow = 0;
  for (size_t i = 0; i < _nrows; ++i) {
        size_t rowLen = rowPtr[i + 1] - rowPtr[i];
        _maxEntriesPerRow = std::max(_maxEntriesPerRow, rowLen);
  }
  
  // Allocate ELL storage (Resize internal arrays)
  _colIdx.assign(_nrows * _maxEntriesPerRow, std::numeric_limits<size_t>::max());
  _values.assign(_nrows * _maxEntriesPerRow, fp_type(0));

  // Fill ELL (column-major ELLPACK layout)
  for (size_t row = 0; row < _nrows; ++row) {
        size_t start = rowPtr[row];
        size_t end   = rowPtr[row + 1];
        for (size_t k = 0; k < end - start; ++k) {
            size_t idx = k * _nrows + row;     // (colMajor)
            _colIdx[idx] = csrColIdx[start + k];
            _values[idx] = csrVals[start + k];
        }
    }

}

//Disassemble:
template <class fp_type>
DisassembledData SparseMatrix_ELL<fp_type>::disassembleStorage() const
{
  DisassembledData out;

  out.nrows = _nrows;
  out.ncols = _ncols;

  // Count number of non-zeros first (optional but avoids reallocation)
  size_t nnz = 0;
  for (size_t r = 0; r < _nrows; ++r) {
      for (size_t k = 0; k < _maxEntriesPerRow; ++k) {
          size_t idx = k * _nrows + r;
          if (_values[idx] != fp_type(0)) {
                ++nnz;
          }
      }
  }

  out.i.reserve(nnz);
  out.j.reserve(nnz);
  out.val.reserve(nnz);

  // Extract COO triplets directly from ELL
  for (size_t r = 0; r < _nrows; ++r) {
      for (size_t k = 0; k < _maxEntriesPerRow; ++k) {
            size_t idx = k * _nrows + r;
            size_t c = _colIdx[idx];
            fp_type v = _values[idx];

            if (v != fp_type(0)) {
                out.i.push_back(r);
                out.j.push_back(c);
                out.val.push_back(v);
            }
      }
  }

  return out;
  
}




} // namespace SpMV
// Need to declare the concrete templates within the library for
// use in code that links to libspmv
template class SpMV::SparseMatrix_ELL<float>;
template class SpMV::SparseMatrix_ELL<double>;
