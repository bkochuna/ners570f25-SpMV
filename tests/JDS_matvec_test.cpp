#include <SpMV.hpp>

#include <vector> // std::vector

// Testing library required for testing (Always include this last!)
#include "unit_test_framework.hpp"

// Use ASSERT(condition) to test if a condition is true.
// Direct comparison of floating point numbers is not recommended, so we define
// ASSERT_NEAR(a, b, epsilon) to test if a and b are within epsilon of each
// other.

// Square matrix test case for JDS matvec implementation
template <typename T> TEST_CASE(square_jds_matvec) {
  // Create a 3x3 JDS matrix
  SpMV::SparseMatrix_JDS<T> jds(3, 3);

  // Set values
  jds.setValue(size_t(0), size_t(0), T(1));
  jds.setValue(size_t(1), size_t(1), T(2));
  jds.setValue(size_t(2), size_t(2), T(3));
  jds.assemble();

  // Create input vector
  std::vector<T> x = {1, 1, 1};
  std::vector<T> y(3, 0);

  // Perform matrix-vector multiplication
  jds.matvec(x, y);

  // Expected output
  std::vector<T> y_expected = {1, 2, 3};

  // Compare results
  for (size_t i = 0; i < y.size(); ++i) {
    ASSERT_NEAR(y[i], y_expected[i], static_cast<T>(1e-6));
  }
} // square_jds_matvec

// Rectangular matrix test case (more rows than columns) for JDS matvec
template <typename T> TEST_CASE(recM_jds_matvec) {
  // Create a 4x2 JDS matrix
  SpMV::SparseMatrix_JDS<T> jds(4, 2);

  // Set values
  jds.setValue(size_t(0), size_t(0), T(2));
  jds.setValue(size_t(1), size_t(1), T(3));
  jds.setValue(size_t(2), size_t(0), T(4));
  jds.setValue(size_t(3), size_t(1), T(5));
  jds.assemble();

  // Create input vector
  std::vector<T> x = {1, 1};
  std::vector<T> y(4, 0);

  // Perform matrix-vector multiplication
  jds.matvec(x, y);

  // Expected output
  std::vector<T> y_expected = {2, 3, 4, 5};

  // Compare results
  for (size_t i = 0; i < y.size(); ++i) {
    ASSERT_NEAR(y[i], y_expected[i], static_cast<T>(1e-6));
  }
}

// Rectangular matrix test case (more columns than rows) for JDS matvec
template <typename T> TEST_CASE(recN_jds_matvec) {

  // Create a 2x4 JDS matrix
  SpMV::SparseMatrix_JDS<T> jds(2, 4);

  // Set values
  jds.setValue(size_t(0), size_t(0), T(1));
  jds.setValue(size_t(0), size_t(3), T(2));
  jds.setValue(size_t(1), size_t(1), T(3));
  jds.setValue(size_t(1), size_t(2), T(4));
  jds.assemble();

  // Create input vector
  std::vector<T> x = {1, 1, 1, 1};
  std::vector<T> y(2, 0);

  // Perform matrix-vector multiplication
  jds.matvec(x, y);

  // Expected output
  std::vector<T> y_expected = {3, 7};

  // Compare results
  for (size_t i = 0; i < y.size(); ++i) {
    ASSERT_NEAR(y[i], y_expected[i], static_cast<T>(1e-6));
  }
}

template <typename T> TEST_SUITE(jds_matvec_suite) {
  TEST(square_jds_matvec<T>);
  TEST(recM_jds_matvec<T>);
  TEST(recN_jds_matvec<T>);
} // add_sub_suite

auto main() -> int {
  // Run the unit tests. If a test fails, the program will print failure info
  // and return 1.

  // Run the templated test suites for floats and doubles
  RUN_SUITE(jds_matvec_suite<float>);
  RUN_SUITE(jds_matvec_suite<double>);
  return 0;
}
