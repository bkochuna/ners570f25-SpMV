#ifndef __SPMV_SparseMatrix_COO__
#define __SPMV_SparseMatrix_COO__

#include <iostream>
#include <stddef.h>
#include "SparseMatrix.hpp"
#include <vector>
#include <cstddef>
#include <iostream>

namespace SpMV
{
    template <class fp_type>
    class SparseMatrix_COO : public SparseMatrix<fp_type>
    {
        protected: 
            std::vector<fp_type> _aij;
            std::vector<size_t> _idx_row;
            std::vector<size_t> _idx_col;

        public:
            SparseMatrix_COO(size_t nrows, size_t ncols)
                : SparseMatrix<fp_type>(nrows, ncols)
            {
                std::cout << "Hello from COO Constructor" << std::endl;
            }

            virtual ~SparseMatrix_COO() noexcept = default;

            void assemble();
    
            void setValue(size_t i, size_t j, fp_type val) {
            // placeholder: do nothing
                (void)i;
                (void)j;
                (void)val;
            }
            // Getters for testing
            size_t nrows() const { return this->_nrows; }
            size_t ncols() const { return this->_ncols; }
            size_t numnz() const { return this->_numnz; }
            MatrixState getState() const { return this->_state; }
            bool buildCoeffEmpty() const { return this->_buildCoeff.empty(); }
            size_t buildCoeffSize() const { return this->_buildCoeff.size(); }
            void view();

            //void    setValue(const size_t i, const size_t j, fp_type val);
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