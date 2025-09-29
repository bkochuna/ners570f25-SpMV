#include "SparseMatrix_COO.hpp"

#include <iostream>

namespace SpMV
{
    SparseMatrix_COO::SparseMatrix_COO() :
         SparseMatrix::SparseMatrix()
    {
        std::cout << "Hello from SparseMatrix_COO Constructor!\n";
    }

} // namespace SpMV
