// Make protected attributes public JUST for this test suite (_idx_col, _idx_row, _aij)
// Kind of janky but should ultimately use friend to access the attributes, once everyones contributions are in
#define protected public
#include <SpMV.hpp>
#undef protected

#include <vector>
#include <algorithm>
#include <set>

#include "unit_test_framework.hpp"

using namespace SpMV;  


// Minimal test to test that _idx_col, _idx_row, and _aij are all consistent with each other and the test matrix
template <typename T>
TEST_CASE(test_mat_features) 
{
	// Initialize a simple 3x3 COO matrix of type T
	using fp_type = T;
	SparseMatrix_COO<fp_type> A(3,3);

	// Set two diagonal, and two off-diagonal terms
	A.setValue(0, 0, 1.0);
	A.setValue(0, 1, 2.0);
	A.setValue(1, 0, 3.0);
	A.setValue(2, 2, 4.0);

	// Assemble using implemented COO assemble function
	A.assemble();

	// Test idx_row, idx_col, and aij all have the same size after assembly
	ASSERT(A._idx_row.size() == A._aij.size());
	ASSERT(A._idx_col.size() == A._aij.size());

	// Check for correct number of non-zero values
	ASSERT(A._idx_row.size() == 4);

	// Check that the max and min values in col and row idx are consistent
	// with the 3 by 3 test
	size_t row_max = *std::max_element(A._idx_row.begin(), A._idx_row.end());
	size_t row_min = *std::min_element(A._idx_row.begin(), A._idx_row.end());
	size_t col_max = *std::max_element(A._idx_col.begin(), A._idx_col.end());
	size_t col_min = *std::min_element(A._idx_col.begin(), A._idx_col.end());

	ASSERT(row_max == 2);
	ASSERT(row_min == 0);
	ASSERT(col_max == 2);
	ASSERT(col_min == 0);

	// Check for idempotence
	size_t nrow_prev = A._idx_row.size();
	size_t ncol_prev = A._idx_col.size();
	size_t naij_prev = A._aij.size();

	A.assemble(); // Call assemble again

	ASSERT(A._idx_row.size() == nrow_prev);
	ASSERT(A._idx_col.size() == ncol_prev);
	ASSERT(A._aij.size() == naij_prev);


}

// Minimal test to test setting diagonal and off-diagonal terms
template <typename T>
TEST_CASE(test_assemble) 
{
	// Initialize a simple 3x3 COO matrix of type T
	using fp_type = T;
	const fp_type eps = 1e-6; // Assertion tolerance
	SparseMatrix_COO<fp_type> A(3,3);

	// Set two diagonal, and two off-diagonal terms
	A.setValue(0, 0, 1.0);
	A.setValue(0, 1, 2.0);
	A.setValue(1, 0, 3.0);
	A.setValue(2, 2, 4.0);

	// Assemble using implemented COO assemble function
	A.assemble();

	// Test the assembled values
	ASSERT_NEAR(A.getValue(0, 0), 1.0, eps);
	ASSERT_NEAR(A.getValue(0, 1), 2.0, eps);
	ASSERT_NEAR(A.getValue(1, 0), 3.0, eps);
	ASSERT_NEAR(A.getValue(2, 2), 4.0, eps);
	ASSERT_NEAR(A.getValue(1, 1), 0.0, eps);
	ASSERT_NEAR(A.getValue(0, 2), 0.0, eps);
	ASSERT_NEAR(A.getValue(2, 0), 0.0, eps);
	ASSERT_NEAR(A.getValue(1, 2), 0.0, eps);
	ASSERT_NEAR(A.getValue(2, 1), 0.0, eps);

	
	// Check for uniqueness
	std::set<std::pair<size_t, size_t>> prev_ind;
	for (size_t k = 0; k < A._aij.size(); ++k) {
		ASSERT(prev_ind.insert({A._idx_row[k], A._idx_col[k]}).second);	
	}

	
	// Check for idempotence
	A.assemble(); // Reassemble

	ASSERT_NEAR(A.getValue(0, 0), 1.0, eps);
	ASSERT_NEAR(A.getValue(0, 1), 2.0, eps);
	ASSERT_NEAR(A.getValue(1, 0), 3.0, eps);
	ASSERT_NEAR(A.getValue(2, 2), 4.0, eps);
	ASSERT_NEAR(A.getValue(1, 1), 0.0, eps);
	ASSERT_NEAR(A.getValue(0, 2), 0.0, eps);
	ASSERT_NEAR(A.getValue(2, 0), 0.0, eps);
	ASSERT_NEAR(A.getValue(1, 2), 0.0, eps);
	ASSERT_NEAR(A.getValue(2, 1), 0.0, eps);

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
	ASSERT(A._idx_row.size() == 0);
	ASSERT(A._idx_col.size() == 0);
	ASSERT(A._aij.size() == 0);


}



// Minimal test to test reassembly
template <typename T>
TEST_CASE(test_reassemble) 
{
	// Initialize a simple 3x3 COO matrix of type T
	using fp_type = T;
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


	// Check for uniqueness
	std::set<std::pair<size_t, size_t>> prev_ind;
	for (size_t k = 0; k < A._aij.size(); ++k) {
		ASSERT(prev_ind.insert({A._idx_row[k], A._idx_col[k]}).second);	
	}

}


// Test Suite for COO Storages
template <typename T>
TEST_SUITE(COO_storage) {
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
