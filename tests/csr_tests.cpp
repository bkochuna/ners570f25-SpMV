
#include <SpMV.hpp>

#include <iostream>
#include <vector> // std::vector

// Testing library required for testing (Always include this last!)
#include "unit_test_framework.hpp"

// Use ASSERT(condition) to test if a condition is true.
// Direct comparison of floating point numbers is not recommended, so we define
// ASSERT_NEAR(a, b, epsilon) to test if a and b are within epsilon of each
// other.

// Create a unit test
template <typename fp_type>
TEST_CASE(assemble_initialized_matrix) 
{

  // Test on an initialized matrix
  SpMV::SparseMatrix_CSR<fp_type> A = SpMV::SparseMatrix_CSR<fp_type>(4,4);
  bool exception_thrown = false;
  try
  {
    A.assemble();
  }
  catch(...)
  {
    exception_thrown = true;
  }
  ASSERT(exception_thrown == true);
}
template <typename fp_type>
TEST_CASE(assemble_assembled_matrix) 
{
  // Test on an assembled matrix
  SpMV::SparseMatrix_CSR<fp_type> A = SpMV::SparseMatrix_CSR<fp_type>(4,4);
  A.assemble();
  bool exception_thrown = false;
  try
  {
    A.assemble();
  }
  catch(...)
  {
    exception_thrown = true;
  }
  ASSERT(exception_thrown == true);
}

template <typename fp_type>
TEST_CASE(assemble_building_matrix) 
{
  // Test on an building matrix
  SpMV::SparseMatrix_CSR<fp_type> A = SpMV::SparseMatrix_CSR<fp_type>(4,4);
  A.setValue(0,0,2);
  bool exception_thrown = false;
  try
  {
    A.assemble();
  }
  catch(...)
  {
    exception_thrown = true;
  }
  ASSERT(exception_thrown == false);
}

template <typename fp_type>
TEST_CASE(disassemble_initialized_matrix) 
{
  // Test on an initialized matrix
  SpMV::SparseMatrix_CSR<fp_type> A = SpMV::SparseMatrix_CSR<fp_type>(4,4);
  bool exception_thrown = false;
  try
  {
    A.disassembleStorage();
  }
  catch(...)
  {
    exception_thrown = true;
  }
  ASSERT(exception_thrown == true);
}
template <typename fp_type>
TEST_CASE(disassemble_assembled_matrix) 
{
  // Test on an assembled matrix
  SpMV::SparseMatrix_CSR<fp_type> A = SpMV::SparseMatrix_CSR<fp_type>(4,4);
  A.assemble();
  bool exception_thrown = false;
  try
  {
    A.disassembleStorage();
  }
  catch(...)
  {
    exception_thrown = true;
  }
  ASSERT(exception_thrown == false);
}
template <typename fp_type>
TEST_CASE(disassemble_building_matrix) 
{
  // Test on an building matrix
  SpMV::SparseMatrix_CSR<fp_type> A = SpMV::SparseMatrix_CSR<fp_type>(4,4);
  A.setValue(0,0,2);
  bool exception_thrown = false;
  try
  {
    A.disassembleStorage();
  }
  catch(...)
  {
    exception_thrown = true;
  }
  ASSERT(exception_thrown == true);
}

template <typename fp_type>
TEST_CASE(disassemble_check)
{
SpMV::SparseMatrix_CSR<fp_type> A = SpMV::SparseMatrix_CSR<fp_type>(4,4);
A.setValue(0,0,2);
A.setValue(3,2,-7);
A.assemble();
A.disassembleStorage();

std::vector <size_t> ia;
std::vector <size_t> ja;
std::vector <fp_type> a;

 ia = A.ia();
 ja = A.ja();
 a = A.a();

ASSERT(ia.size() == 0);
ASSERT(ja.size() == 0);
ASSERT(a.size() == 0);
}

template <typename fp_type>
TEST_CASE(positive_sparse) 
{
  fp_type tol = 1/1000;
  // Initialize variables for testing
  SpMV::SparseMatrix_CSR<fp_type> A = SpMV::SparseMatrix_CSR<fp_type>(4,4);

  //establish rowPtr, colIdx, and value arrays, and use them to generate matix. 
  std::vector <size_t> rowPtrs = {0,2,5,9,10};
  std::vector <size_t> colIdx = {0,1,0,2,3,0,1,2,3,3};
  std::vector <fp_type> value = {1,7,5,3,9,20,2,8,6,6};

  size_t value_ind = 0;
  for(size_t i = 0; i < rowPtrs.size()-1; i++)
  {
    for(size_t j = rowPtrs[i]; j< rowPtrs[i+1]; j++)
    {
      A.setValue(i, colIdx[j], value[value_ind]);
      value_ind ++;
    }
  }
  A.assemble();
  std::vector <size_t> ia;
  std::vector <size_t> ja;
  std::vector <fp_type> a;

  ia = A.ia();
  ja = A.ja();
  a = A.a();

  //std::cout << a.size() << endl;

  //compare sizes of arrays
  ASSERT(ia.size() == rowPtrs.size());
  ASSERT(ja.size() == colIdx.size());
  ASSERT(a.size() == value.size());

  // now compare the assembled arrays to the generating arrays valuewise
    for (size_t k = 0; k < rowPtrs.size(); k++) 
  {
      ASSERT(rowPtrs[k] == ia[k]); 
  }
    for (size_t h = 0; h < colIdx.size(); h++) 
  {
      ASSERT(colIdx[h] == ja[h]); 
      ASSERT_NEAR(value[h], a[h], tol);
  }

  
}

template <typename fp_type>
TEST_CASE(negative_sparse) 
{
  fp_type tol = 1/1000;
  // Initialize variables for testing
  SpMV::SparseMatrix_CSR<fp_type> A = SpMV::SparseMatrix_CSR<fp_type>(4,4);

  //establish rowPtr, colIdx, and value arrays, and use them to generate matix. 
  std::vector <size_t> rowPtrs = {0,2,5,9,10};
  std::vector <size_t> colIdx = {0,1,0,2,3,0,1,2,3,3};
  std::vector <fp_type> value = {-1,-7,-5,-3,-9,-20,-2,-8,-6,-6};

  size_t value_ind = 0;
  for(size_t i = 0; i < rowPtrs.size()-1; i++)
  {
    for(size_t j = rowPtrs[i]; j< rowPtrs[i+1]; j++)
    {
      A.setValue(i, colIdx[j], value[value_ind]);
      value_ind ++;
    }
  }
  A.assemble();
  std::vector <size_t> ia;
  std::vector <size_t> ja;
  std::vector <fp_type> a;

  ia = A.ia();
  ja = A.ja();
  a = A.a();

  //std::cout << a.size() << endl;

  //compare sizes of arrays
  ASSERT(ia.size() == rowPtrs.size());
  ASSERT(ja.size() == colIdx.size());
  ASSERT(a.size() == value.size());

  // now compare the assembled arrays to the generating arrays valuewise
    for (size_t k = 0; k < rowPtrs.size(); k++) 
  {
      ASSERT(rowPtrs[k] == ia[k]); 
  }
    for (size_t h = 0; h < colIdx.size(); h++) 
  {
      ASSERT(colIdx[h] == ja[h]); 
      ASSERT_NEAR(value[h], a[h], tol);
  }

  
}

template <typename fp_type>
TEST_CASE(mixed_sparse) 
{
  fp_type tol = 1/1000;
  // Initialize variables for testing
  SpMV::SparseMatrix_CSR<fp_type> A = SpMV::SparseMatrix_CSR<fp_type>(4,4);

  //establish rowPtr, colIdx, and value arrays, and use them to generate matix. 
  std::vector <size_t> rowPtrs = {0,2,5,9,10};
  std::vector <size_t> colIdx = {0,1,0,2,3,0,1,2,3,3};
  std::vector <fp_type> value = {-1,-7,5,-3,-9,20,2,-8,-6,6};

  size_t value_ind = 0;
  for(size_t i = 0; i < rowPtrs.size()-1; i++)
  {
    for(size_t j = rowPtrs[i]; j< rowPtrs[i+1]; j++)
    {
      A.setValue(i, colIdx[j], value[value_ind]);
      value_ind ++;
    }
  }
  A.assemble();
  std::vector <size_t> ia;
  std::vector <size_t> ja;
  std::vector <fp_type> a;

  ia = A.ia();
  ja = A.ja();
  a = A.a();

  //std::cout << a.size() << endl;

  //compare sizes of arrays
  ASSERT(ia.size() == rowPtrs.size());
  ASSERT(ja.size() == colIdx.size());
  ASSERT(a.size() == value.size());

  // now compare the assembled arrays to the generating arrays valuewise
    for (size_t k = 0; k < rowPtrs.size(); k++) 
  {
      ASSERT(rowPtrs[k] == ia[k]); 
  }
    for (size_t h = 0; h < colIdx.size(); h++) 
  {
      ASSERT(colIdx[h] == ja[h]); 
      ASSERT_NEAR(value[h], a[h], tol);
  }

  
}

template <typename fp_type>
TEST_CASE(dense) 
{
  fp_type tol = 1/1000;
  // Initialize variables for testing
  SpMV::SparseMatrix_CSR<fp_type> A = SpMV::SparseMatrix_CSR<fp_type>(4,4);

  //establish rowPtr, colIdx, and value arrays, and use them to generate matix. 
  std::vector <size_t> rowPtrs = {0,4,8,12,16};
  std::vector <size_t> colIdx = {0,1,2,3,0,1,2,3,0,1,2,3,0,1,2,3};
  std::vector <fp_type> value = {0,1,-2,-3,-4,-5,6,7,8,9,10,11,12,13,14,15};

  size_t value_ind = 0;
  for(size_t i = 0; i < rowPtrs.size()-1; i++)
  {
    for(size_t j = rowPtrs[i]; j< rowPtrs[i+1]; j++)
    {
      A.setValue(i, colIdx[j], value[value_ind]);
      value_ind ++;
    }
  }
  A.assemble();
  std::vector <size_t> ia;
  std::vector <size_t> ja;
  std::vector <fp_type> a;

  ia = A.ia();
  ja = A.ja();
  a = A.a();

  //std::cout << a.size() << endl;

  //compare sizes of arrays
  ASSERT(ia.size() == rowPtrs.size());
  ASSERT(ja.size() == colIdx.size());
  ASSERT(a.size() == value.size());

  // now compare the assembled arrays to the generating arrays valuewise
    for (size_t k = 0; k < rowPtrs.size(); k++) 
  {
      ASSERT(rowPtrs[k] == ia[k]); 
  }
    for (size_t h = 0; h < colIdx.size(); h++) 
  {
      ASSERT(colIdx[h] == ja[h]); 
      ASSERT_NEAR(value[h], a[h], tol);
  }

  
}

template <typename fp_type>
TEST_CASE(big) 
{
  fp_type tol = 1/1000;
  // Initialize variables for testing
  SpMV::SparseMatrix_CSR<fp_type> A = SpMV::SparseMatrix_CSR<fp_type>(50,50);

  //establish rowPtr, colIdx, and value arrays, and use them to generate matix. 
  std::vector <size_t> rowPtrs = {0,7,8,13,16};
  std::vector <size_t> colIdx = {0,1,2,3,4,5,6,3,0,1,25,40,49,11,26,34};
  std::vector <fp_type> value = {0,1,-2,-3,-4,-5,6,7,8,9,10,11,12,13,14,15};

  size_t value_ind = 0;
  for(size_t i = 0; i < rowPtrs.size()-1; i++)
  {
    for(size_t j = rowPtrs[i]; j< rowPtrs[i+1]; j++)
    {
      A.setValue(i, colIdx[j], value[value_ind]);
      value_ind ++;
    }
  }
  A.assemble();
  std::vector <size_t> ia;
  std::vector <size_t> ja;
  std::vector <fp_type> a;

  ia = A.ia();
  ja = A.ja();
  a = A.a();

  //std::cout << a.size() << endl;

  //compare sizes of arrays
  ASSERT(ia.size() == rowPtrs.size());
  ASSERT(ja.size() == colIdx.size());
  ASSERT(a.size() == value.size());

  // now compare the assembled arrays to the generating arrays valuewise
    for (size_t k = 0; k < rowPtrs.size(); k++) 
  {
      ASSERT(rowPtrs[k] == ia[k]); 
  }
    for (size_t h = 0; h < colIdx.size(); h++) 
  {
      ASSERT(colIdx[h] == ja[h]); 
      ASSERT_NEAR(value[h], a[h], tol);
  }

  
}

template <typename fp_type>
TEST_CASE(bigger) 
{
  fp_type tol = 1/1000;
  // Initialize variables for testing
  SpMV::SparseMatrix_CSR<fp_type> A = SpMV::SparseMatrix_CSR<fp_type>(1000,1000);

  //establish rowPtr, colIdx, and value arrays, and use them to generate matix. 
  std::vector <size_t> rowPtrs = {0,7,8,13,16};
  std::vector <size_t> colIdx = {0,1,2,3,4,5,6,3,0,1,25,40,49,11,26,34};
  std::vector <fp_type> value = {0,1,-2,-3,-4,-5,6,7,8,9,10,11,12,13,14,15};

  size_t value_ind = 0;
  for(size_t i = 0; i < rowPtrs.size()-1; i++)
  {
    for(size_t j = rowPtrs[i]; j< rowPtrs[i+1]; j++)
    {
      A.setValue(i, colIdx[j], value[value_ind]);
      value_ind ++;
    }
  }
  A.assemble();
  std::vector <size_t> ia;
  std::vector <size_t> ja;
  std::vector <fp_type> a;

  ia = A.ia();
  ja = A.ja();
  a = A.a();

  //std::cout << a.size() << endl;

  //compare sizes of arrays
  ASSERT(ia.size() == rowPtrs.size());
  ASSERT(ja.size() == colIdx.size());
  ASSERT(a.size() == value.size());

  // now compare the assembled arrays to the generating arrays valuewise
    for (size_t k = 0; k < rowPtrs.size(); k++) 
  {
      ASSERT(rowPtrs[k] == ia[k]); 
  }
    for (size_t h = 0; h < colIdx.size(); h++) 
  {
      ASSERT(colIdx[h] == ja[h]); 
      ASSERT_NEAR(value[h], a[h], tol);
  }

  
}

// Create a test suite
TEST_SUITE(assemble) 
{
  // Run the unit test when the suite is run
  TEST(assemble_initialized_matrix<float>); //this test fails
  TEST(assemble_assembled_matrix<float>);
  TEST(assemble_building_matrix<float>);
  TEST(positive_sparse<float>); //this test fails
  TEST(negative_sparse<float>); //this test fails
  TEST(mixed_sparse<float>); //this test fails
  TEST(dense<float>); //this test fails
  TEST(big<float>); //this test fails
  TEST(bigger<float>);

  TEST(assemble_initialized_matrix<double>); //this test fails
  TEST(assemble_assembled_matrix<double>);
  TEST(assemble_building_matrix<double>);
  TEST(positive_sparse<double>); //this test fails
  TEST(negative_sparse<double>); //this test fails
  TEST(mixed_sparse<double>); //this test fails
  TEST(dense<double>); //this test fails
  TEST(big<double>); //this test fails
  TEST(bigger<double>);
} 

TEST_SUITE(disassemble)
{
  TEST(disassemble_initialized_matrix<float>);
  TEST(disassemble_assembled_matrix<float>);
  TEST(disassemble_building_matrix<float>);
  TEST(disassemble_check<float>);

  TEST(disassemble_initialized_matrix<double>);
  TEST(disassemble_assembled_matrix<double>);
  TEST(disassemble_building_matrix<double>);
  TEST(disassemble_check<double>);
}

auto
main() -> int
{
  // Run the unit tests. If a test fails, the program will print failure info
  // and return 1.
  RUN_SUITE(assemble);
  RUN_SUITE(disassemble);
  return 0; 
}
