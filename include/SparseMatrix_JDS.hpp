#ifndef __SPMV_SparseMatrix_JDS__
#define __SPMV_SparseMatrix_JDS__

#include "SparseMatrix.hpp"
#include <iostream>

//PLEASE READ THE ReaadMe.md first!!!
//PLEASE READ THE ReaadMe.md first!!!
//PLEASE READ THE ReaadMe.md first!!!
//PLEASE READ THE ReaadMe.md first!!!
//PLEASE READ THE ReaadMe.md first!!!
//PLEASE READ THE ReaadMe.md first!!!
//PLEASE READ THE ReaadMe.md first!!!
//PLEASE READ THE ReaadMe.md first!!!
//PLEASE READ THE ReaadMe.md first!!!
//PLEASE READ THE ReaadMe.md first!!!
//PLEASE READ THE ReaadMe.md first!!!

namespace SpMV
{
    template <class fp_type>
    class SparseMatrix_JDS : public SparseMatrix<fp_type>
    {
        public:
            printJDSInputMatrixSummary() const;
            printJDSInputVectorSummary() const;
            printJDSResultVectorSummary() const;
            printJDSMatrixToConsole() const;
            printJDSVectorToConsole() const;
            printJDSResultVectorToConsole() const;
            printJDSMatrixElementToConsole(const size_t row, const size_t column) const;
            printJDSVectorElementToConsole(const size_t index) const;
            printJDSResultVectorElementToConsole(const size_t index) const;
            printJDSMatrixRow(size_t row) const;
            printJDSResultVectorToLogFile(const std::string& filename) const;
    };
}

#endif