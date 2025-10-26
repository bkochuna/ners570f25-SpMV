#include "SparseMatrix_CSR_tests.hpp"
#include "unit_test_framework.hpp"
#include <vector>
#include <cmath>
#include <stdexcept>

using namespace SpMV;

// --- Helper function to create a small CSR matrix for testing ---
template <typename T>
SparseMatrix_CSR<T> createTestCSR() {
    // 3x3 matrix:
    // [1 0 2]
    // [0 0 3]
    // [4 5 6]
    std::vector<size_t> rowPtr = {0, 2, 3, 6};
    std::vector<size_t> colInd = {0, 2, 2, 0, 1, 2};
    std::vector<T> values = {1, 2, 3, 4, 5, 6};
    return SparseMatrix_CSR<T>(3, 3, rowPtr, colInd, values);
}

// --- Test cases ---
template <typename T>
TEST_CASE(testCSRGetters) {
    auto csr = createTestCSR<T>();

    // Test basic info getters
    ASSERT(csr.getNumRows() == 3);
    ASSERT(csr.getNumCols() == 3);
    ASSERT(csr.getNumNonzeros() == 6);

    // Test CSR array getters
    const auto& rowPtr = csr.getRowPtr();
    const auto& colInd = csr.getColInd();
    const auto& values = csr.getValues();

    ASSERT(rowPtr.size() == 4);       // nrows + 1
    ASSERT(colInd.size() == 6);       // nnz
    ASSERT(values.size() == 6);

    // Check actual contents
    ASSERT(rowPtr[0] == 0);
    ASSERT(rowPtr[1] == 2);
    ASSERT(rowPtr[2] == 3);
    ASSERT(rowPtr[3] == 6);

    ASSERT(colInd[0] == 0);
    ASSERT(colInd[1] == 2);
    ASSERT(colInd[2] == 2);
    ASSERT(colInd[3] == 0);
    ASSERT(colInd[4] == 1);
    ASSERT(colInd[5] == 2);

    ASSERT(values[0] == T(1));
    ASSERT(values[1] == T(2));
    ASSERT(values[2] == T(3));
    ASSERT(values[3] == T(4));
    ASSERT(values[4] == T(5));
    ASSERT(values[5] == T(6));
}

// --- Test for uninitialized CSR (should throw exceptions) ---
template <typename T>
TEST_CASE(testCSRUninitializedThrows) {
    SparseMatrix_CSR<T> csr;

    // All getters should throw
    try { csr.getNumRows(); ASSERT(false); } catch (std::runtime_error&) {}
    try { csr.getNumCols(); ASSERT(false); } catch (std::runtime_error&) {}
    try { csr.getNumNonzeros(); ASSERT(false); } catch (std::runtime_error&) {}
    try { csr.getRowPtr(); ASSERT(false); } catch (std::runtime_error&) {}
    try { csr.getColInd(); ASSERT(false); } catch (std::runtime_error&) {}
    try { csr.getValues(); ASSERT(false); } catch (std::runtime_error&) {}
}

// --- Test suite ---
template <typename T>
TEST_SUITE(CSRGettersSuite) {
    TEST((testCSRGetters<T>));
    TEST((testCSRUninitializedThrows<T>));
}

// --- Main ---
auto main() -> int {
    RUN_SUITE(CSRGettersSuite<float>);
    RUN_SUITE(CSRGettersSuite<double>);
    return 0;
}

