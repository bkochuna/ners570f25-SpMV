#ifndef __SPMV_SparseMatrix_CSR__
#define __SPMV_SparseMatrix_CSR__

#include "SparseMatrix.hpp"

namespace SpMV
{
    template <class fp_type>
    class SparseMatrix_CSR : public SparseMatrix<fp_type>
    {
        public:
            SparseMatrix_CSR(const size_t nrows, const size_t ncols);
            void assemble();
    };
}

#endif
