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

            void    setValue(const size_t i, const size_t j, fp_type val);
            fp_type getValue(const size_t i, const size_t j);
    };
}

#endif