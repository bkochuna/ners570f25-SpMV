#include "SparseMatrix_JDS.hpp"
#include <iostream>

namespace SpMV {

// Constructor
template <class fp_type>
SparseMatrix_JDS<fp_type>::SparseMatrix_JDS(const size_t nrows, const size_t ncols)
    : SparseMatrix<fp_type>::SparseMatrix(nrows, ncols) {
    std::cout << "Hello from SparseMatrix_JDS Constructor!" << std::endl;

    // Initialize JDS structure
    _perm.resize(this->_nrows, 0);  // identity permutation
    _jd_ptr.clear();                // j-d diagonal pointers (empty until assemble)
    _colIndices.clear();            // column indices (empty)
    _values.clear();                // values (empty)
    _numDiagonals = 0;              // no diagonals stored yet

    // Optional: mark state as initialized (if consistent with other formats)
    this->_state = MatrixState::initialized;
}

// Destructor (optional debug message)
template <class fp_type>
SparseMatrix_JDS<fp_type>::~SparseMatrix_JDS() {
    std::cout << "Goodbye from SparseMatrix_JDS Destructor!" << std::endl;
}

// Placeholder for assemble
template <class fp_type>
void SparseMatrix_JDS<fp_type>::assemble() {
    std::cout << "JDS assemble() not implemented yet." << std::endl;
}

} // namespace SpMV

template class SpMV::SparseMatrix_JDS<float>;
template class SpMV::SparseMatrix_JDS<double>;
