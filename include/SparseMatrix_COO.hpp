#ifndef __SPMV_SparseMatrix_COO__
#define __SPMV_SparseMatrix_COO__

#include <iostream>
#include <stddef.h>
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

            void    setValue(const size_t i, const size_t j, fp_type val);
            fp_type getValue(const size_t i, const size_t j);

            /**
             * @brief Apply the sparse matrix to input vector x to get b = Ax.
             * 
             * @param x input vector to be left multiplied by matrix
             * @return std::vector<fp_type> 
             */
            std::vector<fp_type> matvec(std::vector<fp_type> x);
    };
}

#endif