// ======= Includes =======
#include <SpMV.hpp>
#include <vector>
#include <map>
#include <iostream>
#include <string>

// Use ASSERT(condition) to test if a condition is true.
// Direct comparison of floating point numbers is not recommended, so we define
// ASSERT_NEAR(a, b, epsilon) to test if a and b are within epsilon of each
// other.

// Testing library required for testing (Always include this last!)
#include "unit_test_framework.hpp"

using namespace SpMV;

// NOTE:
// The following tests assume the SparseMatrix_JDS subclass has been implemented. 
// Furthermore, they assume the existence of the following methods:
//   - setValue(size_t row, size_t col, T value): sets the value at (row, col)
//   - assemble(): finalizes the matrix structure for JDS storage
//   - getValue(size_t row, size_t col): retrieves the value at (row, col)
//   - numnz(): returns the number of non-zero entries
//   - nrows(): returns the number of rows
//   - ncols(): returns the number of columns
// Additionally, it assumes the following public members exist:
//   - values_jds: vector of non-zero values in JDS format
//   - col_indices_jds: vector of column indices corresponding to values_jds
//   - diag_length: vector indicating the length of each diagonal in JDS format


// ======= Macro to run a test with [PASS/FAIL] =======
// Prints out to screen nicely
#define RUN_TEST_CASE(testname) \
    try { \
        testname(); \
        std::cout << "Test: " << #testname << " [PASS]" << std::endl; \
    } catch (...) { \
        std::cout << "Test: " << #testname << " [FAIL]" << std::endl; \
    }

// ======= Test Cases =======
void test_not_assembled()
// Test whether the matrix is successfully assembled before accessing values
// Creates a 3x3 JDS matrix and tries to access the 0,0 element.
// If the matrix is not assembled, it should throw an error
{
    SparseMatrix_JDS<double> A(3,3);
    bool caught = false;
    try {
        A.getValue(0,0);
    } catch (const std::runtime_error &e) {
        caught = true;
    }
    ASSERT(caught);
}

void test_numnz()
// Test whether the number of non-zeros elements is correct after the JDS matrix has been assembled
// Create a 3x3 JDS matrix with the diagonals set to 1,2,3 respectively.
// Test that numnz() returns 3 and that values_jds has size 3.
{
    SparseMatrix_JDS<double> A(3,3);
    A.setValue(0,0,1.0);
    A.setValue(1,1,2.0);
    A.setValue(2,2,3.0);
    A.assemble();

    ASSERT(A.numnz() == 3);
    ASSERT(A.values_jds.size() == 3);
}

void test_no_duplicates()
// Test to ensure there are no duplicate entries in the JDS storage
{
    SparseMatrix_JDS<double> A(3,3);
    A.setValue(0,0,1.0);
    A.setValue(1,1,2.0);
    A.setValue(2,2,3.0);
    A.assemble();

    std::map<std::pair<size_t,size_t>, bool> seen;
    for(size_t k=0; k<A.values_jds.size(); ++k)
    {
        size_t i = getRowIndex(A,k);
        size_t j = A.col_indices_jds[k];
        auto idx = std::make_pair(i,j);
        ASSERT(seen.find(idx) == seen.end());
        seen[idx] = true;
    }
    ASSERT(seen.size() == A.numnz());
}

void test_column_bounds()
// Test to ensure all column indices are within bounds
// Uses the col_indices_jds vector to check column indices
{
    SparseMatrix_JDS<double> A(3,3);
    A.setValue(0,0,1.0);
    A.setValue(1,1,2.0);
    A.setValue(2,2,3.0);
    A.assemble();

    for(size_t j : A.col_indices_jds)
        ASSERT(j < A.ncols());
}

void test_jds_structure()
// Test the JDS structure for a 3x3 matrix 
// Done by checking whether the diagonal length is the same as the number of non-zero entries
{
    SparseMatrix_JDS<double> A(3,3);
    A.setValue(0,0,1.0);
    A.setValue(1,1,2.0);
    A.setValue(2,2,3.0);
    A.assemble();

    for(size_t r=0; r<A.nrows(); ++r)
    {
        size_t count = 0;
        for(size_t k=0; k<A.values_jds.size(); ++k)
            if(getRowIndex(A,k) == r) count++;
        ASSERT(count == A.diag_length[r]);
    }
}

void test_reconstruct_DEN()
// Try to reconstruct an originally dense matrix from its JDS storage
// NOTE: This is a simpler test than presented in the Issue 136.
//       It only tests a small diagonal matrix for brevity.
{
    SparseMatrix_JDS<double> A(3,3);
    A.setValue(0,0,1.0);
    A.setValue(1,1,2.0);
    A.setValue(2,2,3.0);
    A.assemble();

    std::vector<std::vector<double>> DEN(A.nrows(), std::vector<double>(A.ncols(), 0.0));
    for(size_t k=0; k<A.values_jds.size(); ++k)
    {
        // Fill the DEN matrix with values from JDS storage
        size_t i = getRowIndex(A,k);
        size_t j = A.col_indices_jds[k];
        DEN[i][j] = A.values_jds[k];
    }

    for(size_t i=0;i<A.nrows();++i)
        for(size_t j=0;j<A.ncols();++j)
            // Test that the reconstructed DEN matrix matches the JDS within tolerance
            ASSERT_NEAR(DEN[i][j], A.getValue(i,j), 1e-12);
}


// ======= Test Suite ======-
TEST_SUITE(JDS_storage_suite) {
    RUN_TEST_CASE(test_not_assembled);
    RUN_TEST_CASE(test_numnz);
    RUN_TEST_CASE(test_no_duplicates);
    RUN_TEST_CASE(test_column_bounds);
    RUN_TEST_CASE(test_jds_structure);
    RUN_TEST_CASE(test_reconstruct_DEN);
}

auto main() -> int {
	RUN_SUITE(JDS_storage_suite);
	return 0;
}

