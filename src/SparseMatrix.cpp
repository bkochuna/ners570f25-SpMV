#include "SparseMatrix.hpp"

#include <iostream>

namespace SpMV
{
    
    SparseMatrix::SparseMatrix()
    {
        std::cout << "Hello from SparseMatrix Constructor!\n";
    }

    SparseMatrix::~SparseMatrix()
    {
        std::cout << "Hello from SparseMatrix Destructor!\n";
    }

} // namespace SpMV
