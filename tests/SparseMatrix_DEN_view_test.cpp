#include <SpMV.hpp>

#include <vector> // std::vector
#include <sstream>
#include <fstream>
#include <iostream>

// Testing library required for testing (Always include this last!)
#include "unit_test_framework.hpp"

// Use ASSERT(condition) to test if a condition is true.
// Direct comparison of floating point numbers is not recommended, so we define
// ASSERT_NEAR(a, b, epsilon) to test if a and b are within epsilon of each
// other.

using namespace SpMV;

// Test suite for den view function

// template <class fp_type>
// TEST_CASE(view_matrixState) // Assertion errors in code cannot be caught.
// {

//   // Create a matrix but do not initialize it
//   const size_t nrows = 4;
//   const size_t ncols = 4;
//   SparseMatrix_DEN<fp_type> A = new SparseMatrix_DEN(nrows, ncols);

//   // calling view() on an initialized but unassembled matrix should fail.
//   ASSERT( !(A.view()) );

//   A.setValue(1,2,3.);
//   // calling view() on an "building" matrix should fail.
//   ASSERT( !(A.view()) );

//   A.assemble();
//   // calling view() on assembled matrix should succeed.
//   ASSERT( A.view() );

// }
template <class fp_type>
bool compare_withfile(std::ostringstream& oss, std::string& filename)
{
  // This function takes in a stream from .view() outputs, and compare it to the expected output stored in file "filename"
  // search prompt: "c++ verify whether an ostringstream matches a file"

  // compare oss with file
  std::istringstream stream(oss.str());
  std::ifstream file(filename);
  if (!file.is_open()) {
      std::cerr << "Failed to open file: " << filename << std::endl;
      return false;
  }

  std::string fileLine, streamLine;
  int lineNumber = 1;
  while (std::getline(file, fileLine)) {
    if (!std::getline(stream, streamLine)) {
        std::cerr << "Stream has fewer lines than file (failed at line " << lineNumber << ")\n";
        return false;
    }
    if (streamLine != fileLine) {
        std::cerr << "Mismatch at line " << lineNumber << ":\n";
        std::cerr << "File  : [" << fileLine << "]\n";
        std::cerr << "Stream: [" << streamLine << "]\n";
        return false;
    }

    lineNumber++;
  }
  if (std::getline(stream, streamLine)) {
      std::cerr << "Stream has more lines than file (extra line after line " << lineNumber - 1 << "):\n";
      std::cerr << "[" << streamLine << "]\n";
      return false;
  }

  return true;
}

template <class fp_type>
std::ostringstream get_view_output(SparseMatrix_DEN<fp_type> A)
{
  std::ostringstream oss;
  std::streambuf* p_cout_streambuf = std::cout.rdbuf(); // store original std::cout buffer
  std::cout.rdbuf(oss.rdbuf()); // change buffer to oss
  A.view();
  std::cout.rdbuf(p_cout_streambuf); // restore std::cout buffer
  return oss;
}

template <class fp_type>
TEST_CASE(view_dense) 
{

  const size_t nrows = 4;
  const size_t ncols = 4;

  SparseMatrix_DEN<fp_type> A(nrows, ncols);

  fp_type val = 1.;
  // build A with a scattering of values
  for (size_t i=0; i<nrows; i++) {
    for (size_t j=0; j<ncols; j++) {
      A.setValue(i,j,val);
      val += 1.;
    }
  }

  // assemble A
  A.assemble();

  std::ostringstream oss = get_view_output(A);
  std::string compare_file = "den/view_dense.txt";
  ASSERT(compare_withfile<fp_type>(oss, compare_file));

}

template <class fp_type>
TEST_CASE(view_square_small) 
{

  const size_t nrows = 4;
  const size_t ncols = 4;

  SparseMatrix_DEN<fp_type> A(nrows, ncols);

  // build A with a scattering of values
  A.setValue(1,2,3.);
  A.setValue(3,3,static_cast<fp_type>(0.118));

  // assemble A
  A.assemble();

  std::ostringstream oss = get_view_output(A);
  std::string compare_file = "den/view_square_small.txt";
  ASSERT(compare_withfile<fp_type>(oss, compare_file));

}

template <class fp_type>
TEST_CASE(view_square_small_int) 
{

  const size_t nrows = 4;
  const size_t ncols = 4;

  SparseMatrix_DEN<fp_type> A(nrows, ncols);

  // build A with integer values to check implicit type conversion
  A.setValue(1,2,3);
  A.setValue(3,3,18);

  // assemble A
  A.assemble();

  std::ostringstream oss = get_view_output(A);
  std::string compare_file = "den/view_square_small.txt";
  ASSERT(compare_withfile<fp_type>(oss, compare_file));

}

template <class fp_type>
TEST_CASE(view_square_medium) 
{

  const size_t nrows = 400;
  const size_t ncols = 400;

  SparseMatrix_DEN<fp_type> A(nrows, ncols);

  // build A with a scattering of values
  A.setValue(1,2,3.);
  A.setValue(1,2,1.);
  A.setValue(2,1,2.);
  A.setValue(11,2,3.);
  A.setValue(24,72,4.);

  // assemble A
  A.assemble();

  // view A and check output
  std::ostringstream oss = get_view_output(A);
  std::string compare_file = "den/view_square_medium.txt";
  ASSERT(compare_withfile<fp_type>(oss, compare_file));

}

template <class fp_type>
TEST_CASE(view_square_large) 
{

  const size_t nrows = 4000;
  const size_t ncols = 4000;

  SparseMatrix_DEN<fp_type> A(nrows, ncols);

  // build A with a scattering of values
  A.setValue(1,2,1.);
  A.setValue(2,1,2.);
  A.setValue(11,2,3.);
  A.setValue(24,72,4.);
  A.setValue(3785,1024,static_cast<fp_type>(76.558));

  // assemble A
  A.assemble();

  // view A and check output
  std::ostringstream oss = get_view_output(A);
  std::string compare_file = "den/view_square_large.txt";
  ASSERT(compare_withfile<fp_type>(oss, compare_file));

}

template <class fp_type>
TEST_CASE(view_empty) 
{

  const size_t nrows = 4;
  const size_t ncols = 4;

  SparseMatrix_DEN<fp_type> A(nrows, ncols);

  // assemble A when no value is set, should be an empty matrix of zeros.
  A.assemble();

  // view A and check output is a matrix of correct size and all zeros.
  std::ostringstream oss = get_view_output(A);
  std::string compare_file = "den/view_empty.txt";
  ASSERT(compare_withfile<fp_type>(oss, compare_file));

}

template <class fp_type>
TEST_CASE(view_nonsquare) 
{

  const size_t nrows1 = 1;
  const size_t ncols1 = 4;

  SparseMatrix_DEN<fp_type> A(nrows1, ncols1);

  // build A with a scattering of values
  A.setValue(0,2,3.);

  // assemble A when no value is set, should be an empty matrix of zeros.
  A.assemble();

  // view A and check output is a matrix of correct size and all zeros.
  std::ostringstream oss = get_view_output(A);
  std::string compare_file = "den/view_nonsquare_1.txt";
  ASSERT(compare_withfile<fp_type>(oss, compare_file));

  const size_t nrows2 = 4;
  const size_t ncols2 = 1;

  SparseMatrix_DEN<fp_type> B(nrows2, ncols2);

  // build A with a scattering of values
  B.setValue(2,0,3.);

  // assemble A when no value is set, should be an empty matrix of zeros.
  B.assemble();

  // view A and check output is a matrix of correct size and all zeros.
  std::ostringstream oss_2 = get_view_output(B);
  std::string compare_file_2 = "den/view_nonsquare_2.txt";
  ASSERT(compare_withfile<fp_type>(oss_2, compare_file_2));

}

template <class fp_type>
TEST_SUITE(view_suite) 
{
  // TEST(view_matrixState<fp_type>);
  TEST(view_dense<fp_type>);
  TEST(view_square_small<fp_type>);
  TEST(view_square_small_int<fp_type>);
  TEST(view_square_medium<fp_type>);
  TEST(view_square_large<fp_type>);
  TEST(view_empty<fp_type>);
  TEST(view_nonsquare<fp_type>);
}

auto
main() -> int
{
  // Run the unit tests. If a test fails, the program will print failure info
  // and return 1.
  RUN_SUITE(view_suite<double>);
  RUN_SUITE(view_suite<float>);
  return 0; 
}
