#ifndef __SPMV_SparseMatrix_COO__
#define __SPMV_SparseMatrix_COO__

#include "SparseMatrix.hpp"
#include <vector>

namespace SpMV
{
    template <class fp_type>
    class SparseMatrix_COO : public SparseMatrix<fp_type>
    {
        protected: 
            std::vector<fp_type> aij;
            std::vector<size_t> idx_row_;
            std::vector<size_t> idx_col_;

        public:
            SparseMatrix_COO(const size_t nrows, const size_t ncols);
            void assemble();
            std::vector<fp_type> matvec(std::vector<fp_type> x);
    };
}

#endif