#include <SpMV.hpp>

#include <vector> // std::vector

// Testing library required for testing (Always include this last!)
#include "unit_test_framework.hpp"
#include "SparseMatrix_JDS.hpp"

// Use ASSERT(condition) to test if a condition is true.
// Direct comparison of floating point numbers is not recommended, so we define
// ASSERT_NEAR(a, b, epsilon) to test if a and b are within epsilon of each
// other.

// Create a unit test
template <size_t ncol, size_t nrow, typename T>
TEST_CASE(bounds_test) 
{

  T MAT = SparseMatrix_JDS<double>(ncol,nrow);
  ASSERT(MAT.get_nz() == 0);
  ASSERT(MAT.get_ncol() == ncol);
  ASSERT(MAT.get_nrow() == nrow);

  MAT.insert(0,ncol,1.0); 
  ASSERT(MAT.get_nz() == 0); // check out of bounds insertion for columns, should not add size to matrix
  MAT.insert(nrow,0,1.0);
  ASSERT(MAT.get_nz() == 0); // check out of bounds insertion for rows, should not add size to matrix
  MAT.insert(0,0,1.0);
  MAT.insert(0,0,2.0);
  ASSERT(MAT.get_nz() == 1); // check duplicate insertion, should override value, not add additional size

  MAT.assemble();

  ASSERT(MAT.get_nz() == 1); // check behavior again after assembly

}


template < typename T>
TEST_CASE(simple_accessors) 
{

  T MAT = SparseMatrix_JDS<double>(4,5);
  ASSERT(MAT.get_nz() == 0);
  ASSERT(MAT.get_ncol() == 5);
  ASSERT(MAT.get_nrow() == 4);

  MAT.insert(1,1,1.0);
  MAT.insert(3,2,-2.0);
  MAT.insert(3,3,11.0);
  MAT.insert(2,0,1.5);
  MAT.insert(2,1,2.5);
  MAT.insert(2,4,3.5);
  ASSERT(MAT.get_nz() == 6);

  MAT.assemble();

  ASSERT(MAT.get_nz() == 6);

  ASSERT_NEAR(MAT.get_jdiag()[0], 1.5,1e-13);
  ASSERT_NEAR(MAT.get_jdiag()[3], -2.0,1e-13);
  ASSERT_NEAR(MAT.get_jdiag()[5],1.0,1e-13);
  ASSERT(MAT.get_col_ind()[0] == 0);
  ASSERT(MAT.get_col_ind()[3] == 2);
  ASSERT(MAT.get_col_ind()[5] == 1);
  
  ASSERT(MAT.get_jd_ptr()[0]==0);
  ASSERT(MAT.get_jd_ptr()[1]==3);
  ASSERT(MAT.get_jd_ptr()[2]==5);

  ASSERT(MAT.get_perm()[0]==2);
  ASSERT(MAT.get_perm()[3]==3);
  ASSERT(MAT.get_perm()[5]==1);

  ASSERT(MAT.get_maxnz_row()[0]==3);
  ASSERT(MAT.get_maxnz_row()[1]==2);
  ASSERT(MAT.get_maxnz_row()[2]==1);

}

template <typename T>
TEST_SUITE(accessor_suite) 
{
  TEST(bounds_test<4,4,T>);
  TEST(bounds_test<1024,1024,T>);
  TEST(simple_accessors<T>);
  
}

auto
main() -> int
{
  // Run the unit tests. If a test fails, the program will print failure info
  // and return 1.
  RUN_SUITE(accessor_suite<SpMV>);
  return 0; 
}
