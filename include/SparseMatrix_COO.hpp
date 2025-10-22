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
        std::vector<size_t> _idx_col;
        std::vector<size_t> _idx_row;
        std::vector<fp_type> _aij;

        public:
            SparseMatrix_COO(const size_t nrows, const size_t ncols);
            void assemble();
            void view();
    };
}

#endif