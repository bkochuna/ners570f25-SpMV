#include <SpMV.hpp>
#include <vector>
#include "unit_test_framework.hpp"

static constexpr double TOL = 1e-12; //tolerance for calculated vs. expected

// Square matrix (3x3) test
template <typename T>
TEST_CASE(CSR_matvec_square)
{
    // input A =
    // [1 0 2]
    // [0 0 3]
    // [4 5 0]
    const size_t m = 3, n = 3;
    std::vector<size_t> rowPtr = {0, 2, 3, 5};
    std::vector<size_t> colInd = {0, 2, 2, 0, 1};
    std::vector<T> values = {T(1), T(2), T(3), T(4), T(5)};
    SpMV::SparseMatrix_CSR<T> A(m, n, rowPtr, colInd, values);

    std::vector<T> x = {T(1), T(2), T(3)};
    std::vector<T> y(m, T(0);

    A.matvec(x, y);

    // Expected y = [7, 9, 14]
    std::vector<T> expected = {T(7), T(9), T(14)};
    for (size_t i = 0; i < y.size(); ++i)
        ASSERT_NEAR(y[i], expected[i], static_cast<T>(TOL));
}

// Tall matrix (4x2) test
template <typename T>
TEST_CASE(CSR_matvec_tall)
{
    // input A =
    // [1 0]
    // [0 2]
    // [3 0]
    // [0 4]
    const size_t m = 4, n = 2;
    std::vector<size_t> rowPtr = {0, 1, 2, 3, 4};
    std::vector<size_t> colInd = {0, 1, 0, 1};
    std::vector<T> values = {T(1), T(2), T(3), T(4)};
    SpMV::SparseMatrix_CSR<T> A(m, n, rowPtr, colInd, values);

    std::vector<T> x = {T(1), T(2)};
    std::vector<T> y(m, T(0));

    A.matvec(x, y);

    // Expected y = [1, 4, 3, 8]
    std::vector<T> expected = {T(1), T(4), T(3), T(8)};
    for (size_t i = 0; i < y.size(); ++i)
        ASSERT_NEAR(y[i], expected[i], static_cast<T>(TOL));
}

// Wide matrix (2x4) test: nrows < ncols
template <typename T>
TEST_CASE(CSR_matvec_wide)
{
    // input A =
    // [1 0 2 0]
    // [0 3 0 4]
    const size_t m = 2, n = 4;
    std::vector<size_t> rowPtr = {0, 2, 4};
    std::vector<size_t> colInd = {0, 2, 1, 3};
    std::vector<T> values = {T(1), T(2), T(3), T(4)};
    SpMV::SparseMatrix_CSR<T> A(m, n, rowPtr, colInd, values);

    std::vector<T> x = {T(1), T(2), T(3), T(4)};
    std::vector<T> y(m, T(0));

    A.matvec(x, y);

    // Expected y = [7, 22]
    std::vector<T> expected = {T(7), T(22)};
    for (size_t i = 0; i < y.size(); ++i)
        ASSERT_NEAR(y[i], expected[i], static_cast<T>(TOL));
}

// Zero matrix and zero vector tests
template <typename T>
TEST_CASE(CSR_matvec_zero_cases)
{
    // Zero matrix case
    {
        const size_t m = 3, n = 3;
        std::vector<size_t> rowPtr = {0, 0, 0, 0};
        std::vector<size_t> colInd = {};
        std::vector<T> values = {};
        SpMV::SparseMatrix_CSR<T> A(m, n, rowPtr, colInd, values);

        std::vector<T> x = {T(1), T(2), T(3)};
        std::vector<T> y(m, T(0));
        A.matvec(x, y);

        for (auto v : y)
            ASSERT_NEAR(v, T(0), static_cast<T>(TOL));
    }

    // zero vector case
    {
        const size_t m = 3, n = 3;
        std::vector<size_t> rowPtr = {0, 2, 3, 5};
        std::vector<size_t> colInd = {0, 2, 2, 0, 1};
        std::vector<T> values = {T(1), T(2), T(3), T(4), T(5)};
        SpMV::SparseMatrix_CSR<T> A(m, n, rowPtr, colInd, values);

        std::vector<T> x(n, T(0));
        std::vector<T> y(m, T(0));
        A.matvec(x, y);

        for (auto v : y)
            ASSERT_NEAR(v, T(0), static_cast<T>(TOL));
    }
}

// Test suite 
template <typename T>
TEST_SUITE(CSR_matvec_test_suite)
{
    TEST((CSR_matvec_square<T>));
    TEST((CSR_matvec_tall<T>));
    TEST((CSR_matvec_wide<T>));
    TEST((CSR_matvec_zero_cases<T>));
}
// Main
int main()
{
    RUN_SUITE((CSR_matvec_test_suite<float>));
    RUN_SUITE((CSR_matvec_test_suite<double>));
    return 0;
}
