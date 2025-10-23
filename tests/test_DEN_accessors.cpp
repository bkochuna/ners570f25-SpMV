#include <SpMV.hpp>
#include "SparseMatrix_DEN.hpp"
#include "unit_test_framework.hpp"

// Test case: Verify SparseMatrix_DEN accessor methods
TEST_CASE(test_DEN_accessors)
{
    using namespace SpMV;

    // Initialize 3x3 SparseMatrix_DEN
    SparseMatrix_DEN<double> mat(3, 3);

    // Fill with known values
    mat.setValue(0, 0, 1.0);
    mat.setValue(0, 1, 2.0);
    mat.setValue(0, 2, 3.0);
    mat.setValue(1, 0, 4.0);
    mat.setValue(1, 1, 5.0);
    mat.setValue(1, 2, 6.0);
    mat.setValue(2, 0, 7.0);
    mat.setValue(2, 1, 8.0);
    mat.setValue(2, 2, 9.0);

    // Verify numRows and numCols
    ASSERT(mat.numRows() == 3);
    ASSERT(mat.numCols() == 3);

    // Verify getValue returns correct values
    ASSERT_NEAR(mat.getValue(0, 0), 1.0, 1e-12);
    ASSERT_NEAR(mat.getValue(1, 2), 6.0, 1e-12);
    ASSERT_NEAR(mat.getValue(2, 1), 8.0, 1e-12);

    // Test setValue updates the matrix
    mat.setValue(1, 2, 10.0);
    ASSERT_NEAR(mat.getValue(1, 2), 10.0, 1e-12);
}

// Test suite: include this test case
TEST_SUITE(den_accessor_suite)
{
    TEST(test_DEN_accessors);
}

// Main function
auto main() -> int
{
    RUN_SUITE(den_accessor_suite);
    return 0;
}
