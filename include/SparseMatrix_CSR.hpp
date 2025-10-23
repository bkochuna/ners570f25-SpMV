#ifndef __SPMV_SparseMatrix_CSR_Tests__
#define __SPMV_SparseMatrix_CSR_Tests__

#include "SparseMatrix_CSR.hpp"
#include <fstream>
#include <sstream>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>


namespace SpMV
{
    template <class fp_type>
    class SparseMatrix_CSR_Tests
    {
    public:
        bool runTests();
        bool testAccessors(const std::string &matrixFile, const std::string &vecInFile, const std::string &vecOutFile);
        bool compareVectors(const std::vector<double> &v1, const std::vector<double> &v2);
        std::vector<double> openVectorFile(const std::string &filename);
        std::vector<std::vector<double>> openMatrixFile(const std::string &filename);
    };
}

#endif
