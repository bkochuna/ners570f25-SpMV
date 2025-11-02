#define COO_UNIT_TEST 1
#include <SpMV.hpp>

#include <vector>
#include <algorithm>
#include <set>
#include <stdexcept>

#include "unit_test_framework.hpp"

using namespace SpMV;  


// Define the friend class here to be able to access private variables (there is no interface in the accessor methods that supports what is needed for this test)
template <typename T>
class TestSparseMatrixCOO {
public:
  static const std::vector<size_t>& rows(const SpMV::SparseMatrix_COO<T>& A) { return A._idx_row; }
  static const std::vector<size_t>& cols(const SpMV::SparseMatrix_COO<T>& A) { return A._idx_col; }
  static const std::vector<T>&      vals(const SpMV::SparseMatrix_COO<T>& A) { return A._aij; }
};




// Make sure that the matrix will not assemble if it is in the initialized state
template <typename T>
TEST_CASE(test_init_error)
{
	try {
    SparseMatrix_COO<T> A(3,3);
	A.assemble();
    ASSERT(false);     // We should not reach here, it should move to the catch
	}

	catch (const std::runtime_error& e) {
    ASSERT(true);      // Threw as expected
	}



}

// Make sure that the matrix will not assemble if it has already been assembled
template <typename T>
TEST_CASE(test_assemble_error)
{
	// Initialize a simple 3x3 COO matrix of type T
	using fp_type = T;
	using acc = TestSparseMatrixCOO<fp_type>;
	SparseMatrix_COO<fp_type> A(3,3);

	// Set two diagonal, and two off-diagonal terms
	A.setValue(0, 0, 1.0);
	A.setValue(0, 1, 2.0);
	A.setValue(1, 0, 3.0);
	A.setValue(2, 2, 4.0);

	// Assemble using implemented COO assemble function
	A.assemble();

	try {
	A.assemble();
    ASSERT(false);     // We should not reach here, it should have thrown
	}

	catch (const std::runtime_error& e) {
    ASSERT(true);      // Threw as expected
	}


}

// Minimal test to test that _idx_col, _idx_row, and _aij are all consistent with each other and the test matrix
template <typename T>
TEST_CASE(test_mat_features) 
{
	// Initialize a simple 3x3 COO matrix of type T
	using fp_type = T;
	using acc = TestSparseMatrixCOO<fp_type>;
	SparseMatrix_COO<fp_type> A(3,3);

	// Set two diagonal, and two off-diagonal terms
	A.setValue(0, 0, 1.0);
	A.setValue(0, 1, 2.0);
	A.setValue(1, 0, 3.0);
	A.setValue(2, 2, 4.0);

	// Assemble using implemented COO assemble function
	A.assemble();

	// Test idx_row, idx_col, and aij all have the same size after assembly
	ASSERT(acc::rows(A).size() == acc::vals(A).size());
	ASSERT(acc::cols(A).size() == acc::vals(A).size());

	// Check for correct number of non-zero values
	ASSERT(acc::rows(A).size() == 4);

	// Check that the max and min values in col and row idx are consistent
	// with the 3 by 3 test
	size_t row_max = *std::max_element(acc::rows(A).begin(), acc::rows(A).end());
	size_t row_min = *std::min_element(acc::rows(A).begin(), acc::rows(A).end());
	size_t col_max = *std::max_element(acc::cols(A).begin(), acc::cols(A).end());
	size_t col_min = *std::min_element(acc::cols(A).begin(), acc::cols(A).end());

	ASSERT(row_max == 2);
	ASSERT(row_min == 0);
	ASSERT(col_max == 2);
	ASSERT(col_min == 0);


}

// Minimal test to test setting diagonal and off-diagonal terms
template <typename T>
TEST_CASE(test_assemble) 
{
	// Initialize a simple 3x3 COO matrix of type T
	using fp_type = T;
	using acc = TestSparseMatrixCOO<fp_type>;
	const fp_type eps = 1e-6; // Assertion tolerance
	SparseMatrix_COO<fp_type> A(3,3);

	// Set two diagonal, and two off-diagonal terms
	A.setValue(0, 0, 1.0);
	A.setValue(0, 1, 2.0);
	A.setValue(1, 0, 3.0);
	A.setValue(2, 2, -1.0); // Test a negative value

	// Assemble using implemented COO assemble function
	A.assemble();

	// Test the assembled values
	ASSERT_NEAR(A.getValue(0, 0), 1.0, eps);
	ASSERT_NEAR(A.getValue(0, 1), 2.0, eps);
	ASSERT_NEAR(A.getValue(1, 0), 3.0, eps);
	ASSERT_NEAR(A.getValue(2, 2), -1.0, eps);
	ASSERT_NEAR(A.getValue(1, 1), 0.0, eps);
	ASSERT_NEAR(A.getValue(0, 2), 0.0, eps);
	ASSERT_NEAR(A.getValue(2, 0), 0.0, eps);
	ASSERT_NEAR(A.getValue(1, 2), 0.0, eps);
	ASSERT_NEAR(A.getValue(2, 1), 0.0, eps);

	
	// Check for uniqueness - Make sure matrix entries aren't repeated
	std::set<std::pair<size_t, size_t>> prev_ind;
	for (size_t k = 0; k < acc::vals(A).size(); ++k) {
		ASSERT(prev_ind.insert({acc::rows(A)[k], acc::cols(A)[k]}).second);	
	}


	// Check overwrite
	A.setValue(2, 2, 5.0);
	A.assemble();
	ASSERT_NEAR(A.getValue(2, 2), 5.0, eps);


}


// Minimal test to test deassembly
template <typename T>
TEST_CASE(test_deassemble) 
{
	// Initialize a simple 3x3 COO matrix of type T
	using fp_type = T;
	using acc = TestSparseMatrixCOO<fp_type>;
	SparseMatrix_COO<fp_type> A(3,3);

	// Set two diagonal, and two off-diagonal terms
	A.setValue(0, 0, 1.0);
	A.setValue(0, 1, 2.0);
	A.setValue(1, 0, 3.0);
	A.setValue(2, 2, 4.0);

	// Assemble using implemented COO assemble function
	A.assemble();
	A.deassemble();

	// The deassembled values should just be empty if there are no values
	ASSERT(acc::rows(A).size() == 0);
	ASSERT(acc::cols(A).size() == 0);
	ASSERT(acc::vals(A).size() == 0);


}



// Minimal test to test reassembly
template <typename T>
TEST_CASE(test_reassemble) 
{
	// Initialize a simple 3x3 COO matrix of type T
	using fp_type = T;
	using acc = TestSparseMatrixCOO<fp_type>;
	const fp_type eps = 1e-6; // Assertion tolerance
	SparseMatrix_COO<fp_type> A(3,3);

	// Set two diagonal, and two off-diagonal terms
	A.setValue(0, 0, 1.0);
	A.setValue(0, 1, 2.0);
	A.setValue(1, 0, 3.0);
	A.setValue(2, 2, 4.0);

	// Assemble using implemented COO assemble function
	A.assemble();
	A.deassemble();

	// Put new values into _BuildCoeffs
	A.setValue(0, 0, 5.0);
	A.setValue(0, 1, 6.0);
	A.setValue(1, 0, 7.0);
	A.setValue(2, 2, 8.0);

	// Reassemble
	A.assemble();

	// Test that the reassembled values are what is expected
	ASSERT_NEAR(A.getValue(0, 0), 5.0, eps);
	ASSERT_NEAR(A.getValue(0, 1), 6.0, eps);
	ASSERT_NEAR(A.getValue(1, 0), 7.0, eps);
	ASSERT_NEAR(A.getValue(2, 2), 8.0, eps);
	ASSERT_NEAR(A.getValue(1, 1), 0.0, eps);
	ASSERT_NEAR(A.getValue(0, 2), 0.0, eps);
	ASSERT_NEAR(A.getValue(2, 0), 0.0, eps);
	ASSERT_NEAR(A.getValue(1, 2), 0.0, eps);
	ASSERT_NEAR(A.getValue(2, 1), 0.0, eps);


	// Check for uniqueness - Make sure matrix entries aren't repeated
	std::set<std::pair<size_t, size_t>> prev_ind;
	for (size_t k = 0; k < acc::vals(A).size(); ++k) {
		ASSERT(prev_ind.insert({acc::rows(A)[k], acc::cols(A)[k]}).second);	
	}

}


// Test Suite for COO Storages
template <typename T>
TEST_SUITE(COO_storage) {
	TEST(test_init_error<T>);
	TEST(test_assemble_error<T>);
	TEST(test_mat_features<T>);
	TEST(test_assemble<T>);
	TEST(test_deassemble<T>);
	TEST(test_reassemble<T>);
}


// Main
auto
main() -> int
{
  RUN_SUITE(COO_storage<double>);
  RUN_SUITE(COO_storage<float>);
  return 0; 
}
