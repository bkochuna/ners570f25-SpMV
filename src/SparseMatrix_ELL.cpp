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
} // namespace SpMV
// Need to declare the concrete templates within the library for
// use in code that links to libspmv
template class SpMV::SparseMatrix_ELL<float>;
template class SpMV::SparseMatrix_ELL<double>;
