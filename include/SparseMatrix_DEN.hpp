#ifndef __SPMV_SPARSEMATRIX_DEN__
#define __SPMV_SPARSEMATRIX_DEN__

#include "SparseMatrix.hpp"
#include <vector>
#include <stdexcept>
#include <iostream>
#include <cassert>

namespace SpMV
{
    template <class fp_type>
    class SparseMatrix_DEN : public SparseMatrix<fp_type>
    {
    public:
        SparseMatrix_DEN(const size_t nrows, const size_t ncols);
        ~SparseMatrix_DEN();

        void assemble() override;
        std::vector<fp_type> matvec(const std::vector<fp_type>& x) const;

    private:
        std::vector<fp_type> _data;
    };
}

#endif // __SPMV_SPARSEMATRIX_DEN__
