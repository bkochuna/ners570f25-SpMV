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
            SparseMatrix_COO() noexcept
                : SparseMatrix<fp_type>(0,0),
                nrows_(0), ncols_(0), nz_(0),
                aij_(), idx_row_(), idx_col_()
            {}

            SparseMatrix_COO(size_t nrows, size_t ncols)
                : SparseMatrix<fp_type>(nrows, ncols),
                nrows_(nrows),
                ncols_(ncols),
                nz_(0)
            {
                std::cout << "Hello from COO Constructor" << std::endl;
            }

            SparseMatrix_COO(size_t nrows,
                            size_t ncols,
                            size_t nz,
                            const std::vector<fp_type>& aij,
                            const std::vector<size_t>& idx_row,
                            const std::vector<size_t>& idx_col) noexcept
                : SparseMatrix<fp_type>(nrows, ncols),
                nrows_(nrows),
                ncols_(ncols),
                nz_(nz),
                aij_(aij),
                idx_row_(idx_row),
                idx_col_(idx_col)
            {
                std::cout << "Hello from COO Constructor" << std::endl;
            }

            virtual ~SparseMatrix_COO() noexcept = default;

            void assemble();

            private:
                size_t nrows_;
                size_t ncols_;
                size_t nz_;

                std::vector<fp_type> aij_;
                std::vector<size_t> idx_row_;
                std::vector<size_t> idx_col_;
    };
}

#endif