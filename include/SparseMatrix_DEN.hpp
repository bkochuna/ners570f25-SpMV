#ifndef __SPMV_SparseMatrix_DEN__
#define __SPMV_SparseMatrix_DEN__

#include "SparseMatrix.hpp"
#include <memory>
#include <vector>
#include <iostream>
#include <stdexcept>

namespace SpMV
{
    template <class fp_type>
    class SparseMatrix_DEN : public SparseMatrix<fp_type>
    {
    private:
        std::unique_ptr<std::unique_ptr<fp_type[]>[]> _Matrix;

    public:
        SparseMatrix_DEN(const size_t nrows, const size_t ncols);
        ~SparseMatrix_DEN();

        void assemble();
        std::vector<fp_type> matvec(const std::vector<fp_type>& x) const;
    };
}

#endif
