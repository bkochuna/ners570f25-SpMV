#include <SpMV.hpp>

#include <vector> // std::vector

// Testing library required for testing (Always include this last!)
#include "unit_test_framework.hpp"

// Use ASSERT(condition) to test if a condition is true.
// Direct comparison of floating point numbers is not recommended, so we define
// ASSERT_NEAR(a, b, epsilon) to test if a and b are within epsilon of each
// other.

// Create a unit test
template <typename fp_type>
TEST_CASE(ELL_matvec_3x3) 
{

  std::vector<fp_type> const inp = {1.f/3.f, 2.f/3.f, 3.f/3.f};
  std::vector<fp_type> const out = {7.f/3.f, 9.f/3.f, 14.f/3.f};
  
  // Initialize the ELL matrix with 3 rows and 2 non-zeros per row
  size_t nrows = 3;
  size_t ncols = 3;
  SpMV::SparseMatrix_ELL<fp_type> Ell_mat(nrows, ncols);
  
  // Build the matrix
  size_t nnz = 5; // number of non-zeros
  std::vector<size_t> row_indices = {0, 0, 1, 2, 2};
  std::vector<size_t> col_indices = {0, 2, 2, 0, 1};
  std::vector<fp_type> values = {1., 2., 3., 4., 5.};
  for (size_t i = 0; i < nnz; ++i) {
    Ell_mat.setValue(row_indices[i], col_indices[i], values[i]);
  }
  Ell_mat.assemble();

  // Perform the matrix-vector multiplication
  std::vector<fp_type> test_out = Ell_mat.matvec(inp);
  
  for (size_t i = 0; i < x.size(); ++i) {
    ASSERT_NEAR(out[i], test_out[i], 1e-7); 
  }
} // compareVectors

TEST_CASE(matVals_uninitialized) 
{
  std::vector<fp_type> const inp = {1.f/3.f, 2.f/3.f, 3.f/3.f};
  std::vector<fp_type> const out = {7.f/3.f, 9.f/3.f, 14.f/3.f};
  
  // Initialize the ELL matrix with 3 rows and 2 non-zeros per row
  size_t nrows = 3;
  size_t ncols = 3;
  SpMV::SparseMatrix_ELL<fp_type> Ell_mat(nrows, ncols);

  try { Ell_mat.matvec(inp); 
    ASSERT(false); // Should not reach here 
  } catch (...) {
    ASSERT(true); // Exception was thrown as expected
  }

} // matVals_uninitialized

TEST_CASE(matrix_not_assembled) 
{
  std::vector<fp_type> const inp = {1.f/3.f, 2.f/3.f, 3.f/3.f};
  std::vector<fp_type> const out = {7.f/3.f, 9.f/3.f, 14.f/3.f};
  
  // Initialize the ELL matrix with 3 rows and 2 non-zeros per row
  size_t nrows = 3;
  size_t ncols = 3;
  SpMV::SparseMatrix_ELL<fp_type> Ell_mat(nrows, ncols);
  for (size_t i = 0; i < nnz; ++i) {
    Ell_mat.setValue(row_indices[i], col_indices[i], values[i]);
  }

  try { Ell_mat.matvec(inp); 
    ASSERT(false); // Should not reach here 
  } catch (...) {
    ASSERT(true); // Exception was thrown as expected
  }
  
} // matVals_uninitialized

TEST_CASE(matvec_shape_mismatch) 
{
  std::vector<fp_type> const inp = {1.f/3.f, 2.f/3.f, 3.f/3.f, 4.f/3.f};
  
  // Initialize the ELL matrix with 3 rows and 2 non-zeros per row
  size_t nrows = 3;
  size_t ncols = 3;
  SpMV::SparseMatrix_ELL<fp_type> Ell_mat(nrows, ncols);
  for (size_t i = 0; i < nnz; ++i) {
    Ell_mat.setValue(row_indices[i], col_indices[i], values[i]);
  }
  Ell_mat.assemble();

  try { Ell_mat.matvec(inp); 
    ASSERT(false); // Should not reach here 
  } catch (...) {
    ASSERT(true); // Exception was thrown as expected
  }
    
  
} // matVals_uninitialized

template <typename T>
TEST_SUITE(ELL_matvec_test_suite) 
{
  // test single and double precision
  TEST(ELL_matvec_3x3<float>);
  TEST(ELL_matvec_3x3<double>);
  
  // Test unitialized value handling
  TEST((matVals_uninitialized));

  // Test matrix-vector shape mismatch handling
  TEST((matvec_shape_mismatch));

  // Test matrix not assembled
  TEST((matrix_not_assembled));
} // add_sub_suite

auto
main() -> int
{
  // Run the unit tests. If a test fails, the program will print failure info
  // and return 1.
  RUN_SUITE(ELL_matvec_test_suite);
  return 0; 
}
