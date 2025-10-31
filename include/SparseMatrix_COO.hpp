#ifndef __SPMV_SparseMatrix_COO__
#define __SPMV_SparseMatrix_COO__

#include "SparseMatrix.hpp"
#include <vector>
#include <cstddef>
#include <iostream>

namespace SpMV
{
    template <class fp_type>
    class SparseMatrix_COO : public SparseMatrix<fp_type>
    {
        public:
            SparseMatrix_COO(size_t nrows, size_t ncols)
                : SparseMatrix<fp_type>(nrows, ncols)
            {
                std::cout << "Hello from COO Constructor" << std::endl;
            }

            virtual ~SparseMatrix_COO() noexcept = default;

            void assemble();

            void    setValue(const size_t i, const size_t j, fp_type val);
            fp_type getValue(const size_t i, const size_t j);
    };
}

#endif