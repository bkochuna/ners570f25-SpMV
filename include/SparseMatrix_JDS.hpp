#ifndef __SPMV_SparseMatrix_JDS__
#define __SPMV_SparseMatrix_JDS__

#include "SparseMatrix.hpp"
#include "spmv/jds_storage.hpp"
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
            printJDSMatrixElementToConsole(const int row, const int column) const;
            printJDSVectorElementToConsole(const size_t index) const;
            printJDSResultVectorElementToConsole(const size_t index) const;
            printJDSResultVectorToLogFile(const std::string& filename) const;
    };
}

#endif