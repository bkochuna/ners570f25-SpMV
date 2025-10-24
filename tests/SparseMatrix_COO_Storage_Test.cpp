#include <SpMV.hpp>
#include "unit_test_framework.hpp"




// Minimal test to test setting diagonal and off-diagonal terms
template <typename T>
TEST_CASE(test_assemble) 
{
	// Initialize a simple 3x3 COO matrix of type T
	using fp_type = T;
	fp_type eps = 1e-12; // Assertion tolerance
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

}


// Minimal test to test deassembly
template <typename T>
TEST_CASE(test_deassemble) 
{
	// Initialize a simple 3x3 COO matrix of type T
	using fp_type = T;
	fp_type eps = 1e-12; // Assertion tolerance
	fp_type default_val = 0.0 // Default sparse value of 0.0
	SparseMatrix_COO<fp_type> A(3,3);

	// Set two diagonal, and two off-diagonal terms
	A.setValue(0, 0, 1.0);
	A.setValue(0, 1, 2.0);
	A.setValue(1, 0, 3.0);
	A.setValue(2, 2, 4.0);

	// Assemble using implemented COO assemble function
	A.assemble()
	A.deassemble()

	// The deassembled values should just return to some default
	ASSERT_NEAR(A.getValue(0, 0), default_val, eps);
	ASSERT_NEAR(A.getValue(0, 1), default_val, eps);
	ASSERT_NEAR(A.getValue(1, 0), default_val, eps);
	ASSERT_NEAR(A.getValue(2, 2), default_val, eps);
	ASSERT_NEAR(A.getValue(1, 1), default_val, eps);
	ASSERT_NEAR(A.getValue(0, 2), default_val, eps);
	ASSERT_NEAR(A.getValue(2, 0), default_val, eps);
	ASSERT_NEAR(A.getValue(1, 2), default_val, eps);
	ASSERT_NEAR(A.getValue(2, 1), default_val, eps);

}



// Minimal test to test reassembly
template <typename T>
TEST_CASE(test_reassemble) 
{
	// Initialize a simple 3x3 COO matrix of type T
	using fp_type = T;
	fp_type eps = 1e-12; // Assertion tolerance
	fp_type default_val = 0.0
	SparseMatrix_COO<fp_type> A(3,3);

	// Set two diagonal, and two off-diagonal terms
	A.setValue(0, 0, 1.0);
	A.setValue(0, 1, 2.0);
	A.setValue(1, 0, 3.0);
	A.setValue(2, 2, 4.0);

	// Assemble using implemented COO assemble function
	A.assemble()
	A.deassemble()

	// Put new values into _BuildCoeffs
	A.setValue(0, 0, 5.0);
	A.setValue(0, 1, 6.0);
	A.setValue(1, 0, 7.0);
	A.setValue(2, 2, 8.0);

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


}


// Test Suite for COO Storages
template <typename T>
TEST_SUITE(COO_storage) {
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
