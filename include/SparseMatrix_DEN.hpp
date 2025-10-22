#ifndef __SPMV_SparseMatrix_DEN__
#define __SPMV_SparseMatrix_DEN__

#include "SparseMatrix.hpp"
#include <vector>
#include <stdexcept>
#include <iostream>

namespace SpMV
{
    template <class fp_tpye>
    class SparseMatrix_DEN : public SparseMatrix<fp_type>
    {
        public: 
            SparseMatrix_DEN(const size_t nrows, const size_t ncols);
            ~SparseMatrix_Den();

            void assemble() override;
            std::vector<fp_type> matvec(const std::vector<fp_type>& x) const;
        private:
            std::vector<fp_type> _data;
    };
}

#endif
