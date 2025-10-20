#ifndef __SPMV_SparseMatrix_JDS__
#define __SPMV_SparseMatrix_JDS__

#include "SparseMatrix.hpp"

namespace SpMV
{
    template <class fp_type>
    class SparseMatrix_JDS : public SparseMatrix<fp_type>
    {
        public:
            SparseMatrix_JDS(const size_t nrows, const size_t ncols);
            void assemble();
    };
}

#endif