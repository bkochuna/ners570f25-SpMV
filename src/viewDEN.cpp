#include "viewDEN.hpp"
#include <vector>
#include <iomanip>
#include <iostream>

namespace SpMV
{

void viewDEN(const DenseMatrix& A)
{
    if (!A.isAssembled) {
        std::cerr << "[ERROR] Matrix \"" << A.name
                  << "\" has not been assembled. Cannot view contents.\n";
        return;
    }

    std::cout << "=== Viewing Matrix: " << A.name << " ===\n";
    std::cout << "Storage Method : Dense (DEN)\n";
    std::cout << "Dimensions     : " << A.numRows << " x " << A.numCols << "\n";
    std::cout << "Non-zeros      : " << A.numNonZeros << "\n\n";

    std::vector<double> dense(A.numRows * A.numCols, 0.0);
    for (size_t k = 0; k < A.numNonZeros; ++k) {
        dense[A.rowIndices[k] * A.numCols + A.colIndices[k]] = A.values[k];
    }

    size_t w = 1;
    for (auto v : dense) {
        size_t s = std::to_string(v).size();
        if (s > w) w = s;
    }
    w += 1;

    std::cout << "Dense contents (row-major):\n\n";
    for (size_t i = 0; i < A.numRows; ++i) {
        for (size_t j = 0; j < A.numCols; ++j) {
            std::cout << std::setw(w) << dense[i*A.numCols + j];
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

}