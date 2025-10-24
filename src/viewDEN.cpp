#include "viewDEN.hpp"

#include <iostream>

#include <iomanip>

#include <string>

#include <vector>



// Assumes DenseMatrix is declared in another compilation unit

// This file only implements view functionality (no testing framework)



void viewDEN(const DenseMatrix& matrix)

{

    // === 1. Check assembly state ===

    if (!matrix.isAssembled) {

        std::cerr << "[ERROR] Matrix \"" << matrix.name

                  << "\" has not been assembled. Cannot view contents.\n";

        return;

    }



    // === 2. Print matrix summary ===

    std::cout << "=== Viewing Matrix: " << matrix.name << " ===\n";

    std::cout << "Storage Method : Dense (DEN)\n";

    std::cout << "Dimensions     : " << matrix.numRows << " x " << matrix.numCols << "\n";

    std::cout << "Non-zeros      : " << matrix.numNonZeros << "\n\n";



    // === 3. Determine column widths dynamically ===

    size_t maxIWidth = 1, maxJWidth = 1, maxValWidth = 1;

    auto updateWidth = [](size_t& maxWidth, const std::string& s) {

        if (s.size() > maxWidth)

            maxWidth = s.size();

    };



    for (size_t k = 0; k < matrix.numNonZeros; ++k) {

        updateWidth(maxIWidth, std::to_string(matrix.rowIndices[k]));

        updateWidth(maxJWidth, std::to_string(matrix.colIndices[k]));

        updateWidth(maxValWidth, std::to_string(matrix.values[k]));

    }



    // Padding for readability

    maxIWidth += 2;

    maxJWidth += 2;

    maxValWidth += 2;



    // === 4. Print table headers ===

    std::cout << std::left

              << std::setw(maxIWidth) << "I Index"

              << std::setw(maxJWidth) << "J Index"

              << std::setw(maxValWidth) << "Value"

              << "\n";

    std::cout << std::string(maxIWidth + maxJWidth + maxValWidth, '-') << "\n";



    // === 5. Print each non-zero entry ===

    for (size_t k = 0; k < matrix.numNonZeros; ++k) {

        std::cout << std::left

                  << std::setw(maxIWidth) << matrix.rowIndices[k]

                  << std::setw(maxJWidth) << matrix.colIndices[k]

                  << std::setw(maxValWidth) << matrix.values[k]

                  << "\n";

    }



    std::cout << std::endl;

}









































































































































    if (!assembled) {

        std::cerr << "Error: Matrix '" << name

                  << "' is not assembled. Cannot view elements.\n";

        return;

    }



    std::cout << "Matrix Name: " << name << "\n"

              << "Storage Type: DEN (Dense Storage Method)\n"

              << "Dimensions: " << numRows << " x " << numCols << "\n"

              << "Number of Non-Zero Values: " << values.size() << "\n\n";



    size_t widthI = 5, widthJ = 5, widthV = 10;



    for (size_t k = 0; k < values.size(); ++k) {

        widthI = std::max(widthI, std::to_string(rowIndices[k]).length());

        widthJ = std::max(widthJ, std::to_string(colIndices[k]).length());

        std::ostringstream oss;

        oss << std::fixed << std::setprecision(4) << values[k];

        widthV = std::max(widthV, oss.str().length());

    }



    std::cout << std::setw(widthI) << "I Index"

              << std::setw(widthJ + 4) << "J Index"

              << std::setw(widthV + 6) << "Value\n";

    std::cout << std::string(widthI + widthJ + widthV + 12, '-') << "\n";



    for (size_t k = 0; k < values.size(); ++k) {

        std::cout << std::setw(widthI) << rowIndices[k]

                  << std::setw(widthJ + 4) << colIndices[k]

                  << std::setw(widthV + 6) << std::fixed << std::setprecision(4) << values[k]

                  << "\n";

    }

    std::cout << std::endl;

}




























































































































