#include <iostream>

#include <vector>

#include <cassert>

#include <stdexcept>

#include <string>



// ==========================================================

// TEMPORARY PLACEHOLDER — remove once ELLMatrix.hpp/.cpp exist

// ==========================================================



class ELLMatrix {

private:

    size_t numRows, numCols, maxNonZerosPerRow;

    std::vector<size_t> colIndices;   // Flattened storage [row * maxNNZ + j]

    std::vector<double> values;



public:

    ELLMatrix(size_t rows, size_t cols, size_t maxNNZ)

        : numRows(rows), numCols(cols), maxNonZerosPerRow(maxNNZ),

          colIndices(rows * maxNNZ, 0), values(rows * maxNNZ, 0.0) {}



    void set(size_t row, size_t j, size_t col, double val) {

        if (row >= numRows || j >= maxNonZerosPerRow)

            throw std::out_of_range("ELLMatrix::set() out of range");

        colIndices[row * maxNonZerosPerRow + j] = col;

        values[row * maxNonZerosPerRow + j] = val;

    }



    double getValue(size_t row, size_t j) const {

        if (row >= numRows || j >= maxNonZerosPerRow)

            throw std::out_of_range("ELLMatrix::getValue() out of range");

        return values[row * maxNonZerosPerRow + j];

    }



    size_t getColIndex(size_t row, size_t j) const {

        if (row >= numRows || j >= maxNonZerosPerRow)

            throw std::out_of_range("ELLMatrix::getColIndex() out of range");

        return colIndices[row * maxNonZerosPerRow + j];

    }



    size_t getNumRows() const { return numRows; }

    size_t getNumCols() const { return numCols; }

    size_t getMaxNNZPerRow() const { return maxNonZerosPerRow; }

};



// ==========================================================

// Test Utility

// ==========================================================

void printTestResult(const std::string& testName, bool passed) {

    std::cout << (passed ? "[PASS] " : "[FAIL] ") << testName << std::endl;

}



// ==========================================================

// Unit Tests

// ==========================================================

void test_validAccessorReads() {

    ELLMatrix A(3, 3, 2);

    A.set(0, 0, 0, 1.1);

    A.set(0, 1, 1, 2.2);

    A.set(1, 0, 2, 3.3);



    assert(A.getValue(0, 0) == 1.1);

    assert(A.getColIndex(1, 0) == 2);

    printTestResult("Valid data access retrieves correct values", true);

}



void test_outOfRangeAccessThrows() {

    ELLMatrix A(2, 2, 2);

    bool caught = false;

    try {

        A.getValue(3, 0); // invalid row

    } catch (const std::out_of_range&) {

        caught = true;

    }

    assert(caught);

    printTestResult("Out-of-range accessor throws exception", caught);

}



void test_paddedZeroAccess() {

    ELLMatrix A(2, 2, 3); // has padding

    A.set(0, 0, 0, 5.0);

    assert(A.getValue(0, 1) == 0.0); // default

    assert(A.getColIndex(0, 1) == 0); // default

    printTestResult("Padded zero entries handled correctly", true);

}



void test_immutableAccessors() {

    ELLMatrix A(1, 1, 1);

    A.set(0, 0, 0, 9.9);



    const ELLMatrix& constRef = A;

    double val = constRef.getValue(0, 0);

    size_t col = constRef.getColIndex(0, 0);



    assert(val == 9.9);

    assert(col == 0);

    printTestResult("Accessors are read-only (const safe)", true);

}



void test_edgeCases() {

    // Empty

    ELLMatrix empty(0, 0, 0);

    assert(empty.getNumRows() == 0 && empty.getNumCols() == 0);

    printTestResult("Empty matrix handled correctly", true);



    // 1x1

    ELLMatrix single(1, 1, 1);

    single.set(0, 0, 0, 42.0);

    assert(single.getValue(0, 0) == 42.0);

    printTestResult("1x1 matrix handled correctly", true);



    // Fully dense

    ELLMatrix dense(2, 2, 2);

    dense.set(0, 0, 0, 1.0);

    dense.set(0, 1, 1, 2.0);

    dense.set(1, 0, 0, 3.0);

    dense.set(1, 1, 1, 4.0);

    assert(dense.getValue(1, 1) == 4.0);

    printTestResult("Fully dense ELL matrix handled correctly", true);

}



void test_invalidInputHandling() {

    ELLMatrix A(2, 2, 2);

    bool caught = false;

    try {

        size_t badIndex = static_cast<size_t>(-1);

        A.getValue(badIndex, 0);

    } catch (const std::out_of_range&) {

        caught = true;

    }

    assert(caught);

    printTestResult("Invalid index input handled gracefully", caught);

}



// ==========================================================

// Main

// ==========================================================

int main() {

    std::cout << "=== Running ELL Accessor Unit Tests ===\n";



    test_validAccessorReads();

    test_outOfRangeAccessThrows();

    test_paddedZeroAccess();

    test_immutableAccessors();

    test_edgeCases();

    test_invalidInputHandling();



    std::cout << "=== All tests complete ===\n";

    return 0;

}


