#include "SparseMatrix_COO.hpp"
#include <iostream>

namespace SpMV {

template<class fp_type>
SparseMatrix_COO<fp_type>::SparseMatrix_COO(size_t nrows, size_t ncols)
  : SparseMatrix<fp_type>(nrows, ncols)   // <-- call base-class ctor
{

}

template<class fp_type>
void SparseMatrix_COO<fp_type>::assemble()
{
    std::cout << "Hello from COO assemble" << std::endl;
    this->_assembled = true;  // mark finalized (declared in the header)
}

// Explicit template instantiations (types you use)
template class SparseMatrix_COO<double>;
template class SparseMatrix_COO<float>;

} // namespace SpMV

