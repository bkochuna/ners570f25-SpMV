#ifndef __SPMV_SparseMatrix_CSR__
#define __SPMV_SparseMatrix_CSR__

#include "SparseMatrix.hpp"
#include <iostream>

namespace SpMV
{
    template <class fp_type>
    class SparseMatrix_CSR : public SparseMatrix<fp_type>
    {
        public:
            printCSRToCounsole();
            printCSRElementToConsole(const size_t row, const size_t column);
            printCSRToLogFile(const std::string& filename);
    };
}

#endif