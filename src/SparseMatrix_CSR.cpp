#include "SparseMatrix_CSR.hpp"
#include <stdexcept>

namespace SpMV {

template <class fp_type>
void SparseMatrix_CSR<fp_type>::matvec(const std::vector<fp_type>& x, std::vector<fp_type>& y)
{
    
    // Check the input vectors are the same size
    if (x.size() != this->_ncols)
        throw std::invalid_argument("matvec size error for x");
    if (y.size() != this->_nrows)
        throw std::invalid_argument("matvec size error for y");
    
    // Initialize result vector to equal zeros
    std::fill(y.begin(),y.end(),0);

    const index_type R = this->_nrows;

    // Preform matvec operation
    for (index_type i = 0; i < R; i++){
        // solve for the start and end indicies
        index_type row_start = _ia[i]; 
        index_type row_end = _ia[i+1];

        // loop through rows
        for (index_type k = row_start; k < row_end; ++k){
            index_type col = _ja[k];
            fp_type val = _a[k];
            y[i] += val * x[col];
        }
    }
}

// template information
template class SparseMatrix_CSR<double>;
template class SparseMatrix_CSR<float>;

} // namespace SpMV
