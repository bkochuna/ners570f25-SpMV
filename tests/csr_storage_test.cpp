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

TEST_CASE(assemble_initialized_matrix) 
{

  // Test on an initialized matrix
  SpMV::SparseMatrix_CSR<double> A = SpMV::SparseMatrix_CSR<double>(4,4);
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
TEST_CASE(assemble_assembled_matrix) 
{

  // Test on an assembled matrix
  SpMV::SparseMatrix_CSR<double> A = SpMV::SparseMatrix_CSR<double>(4,4);
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
TEST_CASE(assemble_building_matrix) 
{

  // Test on an building matrix
  SpMV::SparseMatrix_CSR<double> A = SpMV::SparseMatrix_CSR<double>(4,4);
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

TEST_CASE(disassemble_initialized_matrix) 
{

  // Test on an initialized matrix
  SpMV::SparseMatrix_CSR<double> A = SpMV::SparseMatrix_CSR<double>(4,4);
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
TEST_CASE(disassemble_assembled_matrix) 
{

  // Test on an assembled matrix
  SpMV::SparseMatrix_CSR<double> A = SpMV::SparseMatrix_CSR<double>(4,4);
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
TEST_CASE(disassemble_building_matrix) 
{

  // Test on an building matrix
  SpMV::SparseMatrix_CSR<double> A = SpMV::SparseMatrix_CSR<double>(4,4);
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

TEST_CASE(disassemble_check)
{

SpMV::SparseMatrix_CSR<double> A = SpMV::SparseMatrix_CSR<double>(4,4);
A.setValue(0,0,2);
A.setValue(3,2,-7);
A.assemble();
A.disassembleStorage();

std::vector <size_t> ia;
std::vector <size_t> ja;
std::vector <double> a;

 ia = A.ia();
 ja = A.ja();
 a = A.a();

ASSERT(ia.size() == 0);
ASSERT(ja.size() == 0);
ASSERT(a.size() == 0);
}

TEST_CASE(positive_sparse) 
{

  // Initialize variables for testing
  SpMV::SparseMatrix_CSR<double> A = SpMV::SparseMatrix_CSR<double>(4,4);

  //establish rowPtr, colIdx, and value arrays, and use them to generate matix. 
  std::vector <size_t> rowPtrs = {0,2,5,9,10};
  std::vector <size_t> colIdx = {0,1,0,2,3,0,1,2,3,3};
  std::vector <double> value = {1,7,5,3,9,20,2,8,6,6};

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
  std::vector <double> a;

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
      ASSERT_NEAR(value[h], a[h], 1e-3);
  }

  
}

TEST_CASE(negative_sparse) 
{

  // Initialize variables for testing
  SpMV::SparseMatrix_CSR<double> A = SpMV::SparseMatrix_CSR<double>(4,4);

  //establish rowPtr, colIdx, and value arrays, and use them to generate matix. 
  std::vector <size_t> rowPtrs = {0,2,5,9,10};
  std::vector <size_t> colIdx = {0,1,0,2,3,0,1,2,3,3};
  std::vector <double> value = {-1,-7,-5,-3,-9,-20,-2,-8,-6,-6};

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
  std::vector <double> a;

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
      ASSERT_NEAR(value[h], a[h], 1e-3);
  }

  
}

TEST_CASE(mixed_sparse) 
{

  // Initialize variables for testing
  SpMV::SparseMatrix_CSR<double> A = SpMV::SparseMatrix_CSR<double>(4,4);

  //establish rowPtr, colIdx, and value arrays, and use them to generate matix. 
  std::vector <size_t> rowPtrs = {0,2,5,9,10};
  std::vector <size_t> colIdx = {0,1,0,2,3,0,1,2,3,3};
  std::vector <double> value = {-1,-7,5,-3,-9,20,2,-8,-6,6};

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
  std::vector <double> a;

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
      ASSERT_NEAR(value[h], a[h], 1e-3);
  }

  
}
TEST_CASE(dense) 
{

  // Initialize variables for testing
  SpMV::SparseMatrix_CSR<double> A = SpMV::SparseMatrix_CSR<double>(4,4);

  //establish rowPtr, colIdx, and value arrays, and use them to generate matix. 
  std::vector <size_t> rowPtrs = {0,4,8,12,16};
  std::vector <size_t> colIdx = {0,1,2,3,0,1,2,3,0,1,2,3,0,1,2,3};
  std::vector <double> value = {0,1,-2,-3,-4,-5,6,7,8,9,10,11,12,13,14,15};

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
  std::vector <double> a;

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
      ASSERT_NEAR(value[h], a[h], 1e-3);
  }

  
}
TEST_CASE(big) 
{

  // Initialize variables for testing
  SpMV::SparseMatrix_CSR<double> A = SpMV::SparseMatrix_CSR<double>(50,50);

  //establish rowPtr, colIdx, and value arrays, and use them to generate matix. 
  std::vector <size_t> rowPtrs = {0,7,8,13,16};
  std::vector <size_t> colIdx = {0,1,2,3,4,5,6,3,0,1,25,40,49,11,26,34};
  std::vector <double> value = {0,1,-2,-3,-4,-5,6,7,8,9,10,11,12,13,14,15};

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
  std::vector <double> a;

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
      ASSERT_NEAR(value[h], a[h], 1e-3);
  }

  
}
TEST_CASE(bigger) 
{

  // Initialize variables for testing
  SpMV::SparseMatrix_CSR<double> A = SpMV::SparseMatrix_CSR<double>(1000,1000);

  //establish rowPtr, colIdx, and value arrays, and use them to generate matix. 
  std::vector <size_t> rowPtrs = {0,7,8,13,16};
  std::vector <size_t> colIdx = {0,1,2,3,4,5,6,3,0,1,25,40,49,11,26,34};
  std::vector <double> value = {0,1,-2,-3,-4,-5,6,7,8,9,10,11,12,13,14,15};

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
  std::vector <double> a;

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
      ASSERT_NEAR(value[h], a[h], 1e-3);
  }

  
}

// Create a test suite
TEST_SUITE(assemble) 
{
  // Run the unit test when the suite is run
  TEST(assemble_initialized_matrix); //this test fails
  TEST(assemble_assembled_matrix);
  TEST(assemble_building_matrix);
  TEST(positive_sparse); //this test fails
  TEST(negative_sparse); //this test fails
  TEST(mixed_sparse); //this test fails
  TEST(dense); //this test fails
  TEST(big); //this test fails
  TEST(bigger)
} 

TEST_SUITE(disassemble)
{
  TEST(disassemble_initialized_matrix);
  TEST(disassemble_assembled_matrix);
  TEST(disassemble_building_matrix);
  TEST(disassemble_check);
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
