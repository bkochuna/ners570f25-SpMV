#ifndef __SPMV_SPARSEMATRIX_DEN__
#define __SPMV_SPARSEMATRIX_DEN__

#include "SparseMatrix.hpp"
#include <memory>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <cassert>

namespace SpMV
{
    template <class fp_type>
    class SparseMatrix_DEN : public SparseMatrix<fp_type>
    {
        private:
            std::unique_ptr<std::unique_ptr<fp_type[]>[]> _Matrix; // Declare Matrix


        public:
            SparseMatrix_DEN(const size_t nrows, const size_t ncols); // Constructor
            ~SparseMatrix_DEN(); // Destructor
            
            void assemble() override;
            void assembleStorage();
            void disassembleStorage();
            fp_type getValue(const size_t i, const size_t j);
            std::vector<fp_type>* matvec(const std::vector<fp_type>& x) const;
    };

}

#endif
