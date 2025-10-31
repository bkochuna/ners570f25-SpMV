#ifndef __SPMV_SparseMatrix_COO__
#define __SPMV_SparseMatrix_COO__

#include "SparseMatrix.hpp"

namespace SpMV
{
    template <class fp_type>
    class SparseMatrix_COO : public SparseMatrix<fp_type>
    {
        public:
            SparseMatrix_COO(const size_t nrows, const size_t ncols);
            void assemble();
    
    
            // Getters for testing
            size_t nrows() const { return this->_nrows; }
            size_t ncols() const { return this->_ncols; }
            size_t numnz() const { return this->_numnz; }
            MatrixState getState() const { return this->_state; }
            bool buildCoeffEmpty() const { return this->_buildCoeff.empty(); }
            size_t buildCoeffSize() const { return this->_buildCoeff.size(); }
    };
}

#endif