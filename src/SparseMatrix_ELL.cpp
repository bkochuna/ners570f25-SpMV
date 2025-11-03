#include "SparseMatrix_ELL.hpp"

#include <iostream>
#include <stddef.h>

using namespace std;

namespace SpMV {

// Constructor
template <class fp_type>
SparseMatrix_ELL<fp_type>::SparseMatrix_ELL(const size_t nrows,
                                            const size_t ncols)
    : SparseMatrix<fp_type>(nrows, ncols) {
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
//Accessor implementation
// --- Accessors Implementation ---
template <class fp_type>
const std::vector<int>& SparseMatrix_ELL<fp_type>::getColInd() const
{
    if (this->_nrows == 0 || this->_ncols == 0)
        throw std::runtime_error("ELL: getColInd() called on unconstructed or zero-size matrix.");
    if (this->_state == MatrixState::undefined)
        throw std::runtime_error("ELL: getColInd() called on uninitialized matrix.");
    return _colIndices;
}

template <class fp_type>
const std::vector<fp_type>& SpMV::SparseMatrix_ELL<fp_type>::getValues() const
{
    if (this->_nrows == 0 || this->_ncols == 0)
        throw std::runtime_error("ELL: getValues() called on unconstructed or zero-size matrix.");
    if (this->_state == MatrixState::undefined)
        throw std::runtime_error("ELL: getValues() called on uninitialized matrix.");
    return _values;
}

template <class fp_type>
size_t SpMV::SparseMatrix_ELL<fp_type>::getMaxNnzPerRow() const
{
    if (this->_nrows == 0 || this->_ncols == 0)
        throw std::runtime_error("ELL: getMaxNnzPerRow() called on unconstructed or zero-size matrix.");
    if (this->_state == MatrixState::undefined)
        throw std::runtime_error("ELL: getMaxNnzPerRow() called on uninitialized matrix.");
    return _maxNnzPerRow;
}
} // namespace SpMV
// Need to declare the concrete templates within the library for
// use in code that links to libspmv
template class SpMV::SparseMatrix_ELL<float>;
template class SpMV::SparseMatrix_ELL<double>;
