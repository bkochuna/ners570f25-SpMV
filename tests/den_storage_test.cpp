// tests/den_storage_tests.cpp
#include <SpMV.hpp>
#include "SparseMatrix_DEN.hpp"

#include <vector>
#include <limits>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <cstddef>

// Testing library required for testing (always include this last)
#include "unit_test_framework.hpp"

// Local test helper
#include "den_storage_tests.hpp"

// Helper definition
template <typename T>
std::vector<T> make_dense_row_major(
    std::size_t m, std::size_t n,
    const std::vector<std::size_t>& I,
    const std::vector<std::size_t>& J,
    const std::vector<T>& V)
{
  std::vector<T> dense(m * n, T{0});
  for (std::size_t k = 0; k < V.size(); ++k) {
    const std::size_t i = I[k], j = J[k];
    dense[i * n + j] = V[k]; // overwrite policy
  }
  return dense;
}

// -----------------------------------------------------------------------------
// Assembly required before access
// Coverage: Task "If the DEN matrix is not assembled, stop execution."
// -----------------------------------------------------------------------------
TEST_CASE(den_requires_assembly_before_access)
{
  SpMV::SparseMatrix_DEN<double> A(3, 3);

  bool threw = false;
  try {
    (void)A.values(); // access prior to assembleStorage() should be blocked
  } catch (const std::exception&) {
    threw = true;
  }
  ASSERT(threw && "Access prior to assembly did not stop execution");

  std::cout << "[Pass] Assembly required before access\n";
}

// -----------------------------------------------------------------------------
// Dimensions equal m × n
// Coverage: Task "Total stored values = m × n."
// -----------------------------------------------------------------------------
TEST_CASE(den_storage_dimensions_mn)
{
  const std::size_t m = 4, n = 5;
  std::vector<std::size_t> I = {0, 0, 1, 3};
  std::vector<std::size_t> J = {0, 4, 2, 1};
  std::vector<double>       V = {1.0, 2.0, 3.0, 4.0};

  SpMV::SparseMatrix_DEN<double> A(m, n);
  A.assembleStorage(I, J, V, m, n);

  ASSERT(A.values().size() == m * n);
  std::cout << "[Pass] Dimensions check (m × n)\n";
}

// -----------------------------------------------------------------------------
// All indices are within bounds
// Coverage: Task "All indices are within bounds."
// -----------------------------------------------------------------------------
TEST_CASE(den_index_bounds)
{
  const std::size_t m = 3, n = 4;

  // Valid: edge indices
  {
    std::vector<std::size_t> I = {0, 0, m - 1, m - 1};
    std::vector<std::size_t> J = {0, n - 1, 0, n - 1};
    std::vector<double>       V = {1, 2, 3, 4};

    SpMV::SparseMatrix_DEN<double> A(m, n);
    A.assembleStorage(I, J, V, m, n);
    ASSERT(A.values().size() == m * n);
  }

  // Invalid: out-of-bounds index
  {
    std::vector<std::size_t> I = {0, m}; // m is out of bounds
    std::vector<std::size_t> J = {0, 1};
    std::vector<double>       V = {1, 2};

    SpMV::SparseMatrix_DEN<double> A(m, n);
    bool threw = false;
    try { A.assembleStorage(I, J, V, m, n); }
    catch (const std::exception&) { threw = true; }
    ASSERT(threw && "Out-of-bounds indices were not rejected");
  }

  std::cout << "[Pass] Index bounds validation\n";
}

// -----------------------------------------------------------------------------
// Non-zeros match input; zeros are zero; no NaN/Inf
// Coverage:
// - Task "Non-zeros match input (no missing/extra)."
// - Task "Zero elements are zero, no NaN/Inf."
// -----------------------------------------------------------------------------
TEST_CASE(den_values_match_and_zeros_clean)
{
  const std::size_t m = 4, n = 4;
  std::vector<std::size_t> I = {0, 1, 2, 3, 1};
  std::vector<std::size_t> J = {0, 1, 2, 3, 3};
  std::vector<double>       V = {10, 20, 30, 40, 50};

  const auto expected = make_dense_row_major<double>(m, n, I, J, V);

  SpMV::SparseMatrix_DEN<double> A(m, n);
  A.assembleStorage(I, J, V, m, n);

  const auto& dense = A.values();
  ASSERT(dense.size() == m * n);

  for (std::size_t idx = 0; idx < dense.size(); ++idx) {
    ASSERT(dense[idx] == expected[idx]);
  }

  for (double x : dense) {
    ASSERT(!(std::isnan(x) || std::isinf(x)));
  }

  std::cout << "[Pass] Non-zeros match input; zeros are zero; no NaN/Inf\n";
}

// -----------------------------------------------------------------------------
// Intentional incorrect or invalid input
// Coverage: Task "Test behavior with intentionally incorrect or invalid input
// to ensure safe error handling."
// -----------------------------------------------------------------------------
TEST_CASE(den_invalid_inputs_fail_safely)
{
  const std::size_t m = 3, n = 3;

  // Mismatched vector lengths
  {
    std::vector<std::size_t> I = {0, 1, 2};
    std::vector<std::size_t> J = {0, 1}; // mismatch
    std::vector<double>       V = {1, 2, 3};

    SpMV::SparseMatrix_DEN<double> A(m, n);
    bool threw = false;
    try { A.assembleStorage(I, J, V, m, n); }
    catch (const std::exception&) { threw = true; }
    ASSERT(threw && "Mismatched input vector lengths were not rejected");
  }

  // Dimensions conflicting with constructor
  {
    std::vector<std::size_t> I = {0};
    std::vector<std::size_t> J = {0};
    std::vector<double>       V = {1};
    SpMV::SparseMatrix_DEN<double> A(m, n);
    bool threw = false;
    try { A.assembleStorage(I, J, V, m + 1, n); } // wrong dims supplied
    catch (const std::exception&) { threw = true; }
    ASSERT(threw && "Conflicting assemble dimensions were not rejected");
  }

  // NaN / Inf values in inputs
  {
    std::vector<std::size_t> I = {0, 1};
    std::vector<std::size_t> J = {0, 1};
    std::vector<double>       V = {
      std::numeric_limits<double>::quiet_NaN(),
      std::numeric_limits<double>::infinity()
    };
    SpMV::SparseMatrix_DEN<double> A(m, n);
    bool threw = false;
    try { A.assembleStorage(I, J, V, m, n); }
    catch (const std::exception&) { threw = true; }
    ASSERT(threw && "NaN/Inf inputs were not rejected");
  }

  std::cout << "[Pass] Invalid inputs handled safely\n";
}

// -----------------------------------------------------------------------------
// Extremely small matrices
// Coverage: Task "Include extremely small matrices to ensure stability and
// correct handling."
// -----------------------------------------------------------------------------
TEST_CASE(den_extremely_small_matrices)
{
  // 1x1
  {
    const std::size_t m = 1, n = 1;
    std::vector<std::size_t> I = {0};
    std::vector<std::size_t> J = {0};
    std::vector<double>       V = {7.0};

    SpMV::SparseMatrix_DEN<double> A(m, n);
    A.assembleStorage(I, J, V, m, n);

    ASSERT(A.values().size() == 1);
    ASSERT(A.values()[0] == 7.0);
  }

  // 1xN
  {
    const std::size_t m = 1, n = 5;
    std::vector<std::size_t> I = {0, 0};
    std::vector<std::size_t> J = {1, 4};
    std::vector<double>       V = {2.0, 5.0};

    const auto expected = make_dense_row_major<double>(m, n, I, J, V);

    SpMV::SparseMatrix_DEN<double> A(m, n);
    A.assembleStorage(I, J, V, m, n);

    ASSERT(A.values().size() == m * n);
    for (std::size_t k = 0; k < expected.size(); ++k) ASSERT(A.values()[k] == expected[k]);
  }

  // Mx1
  {
    const std::size_t m = 4, n = 1;
    std::vector<std::size_t> I = {1, 3};
    std::vector<std::size_t> J = {0, 0};
    std::vector<double>       V = {9.0, 4.0};

    const auto expected = make_dense_row_major<double>(m, n, I, J, V);

    SpMV::SparseMatrix_DEN<double> A(m, n);
    A.assembleStorage(I, J, V, m, n);

    ASSERT(A.values().size() == m * n);
    for (std::size_t k = 0; k < expected.size(); ++k) ASSERT(A.values()[k] == expected[k]);
  }

  std::cout << "[Pass] Extremely small matrices handled correctly\n";
}

// -----------------------------------------------------------------------------
// Very large matrices (bounded for CI/runtime)
// Coverage: Task "Include extremely small and very large matrices to ensure
// stability and correct handling."
// -----------------------------------------------------------------------------
TEST_CASE(den_very_large_matrix_smoke)
{
  const std::size_t m = 256, n = 256;

  // Diagonal pattern: A[i,i] = 1.0
  std::vector<std::size_t> I, J;
  std::vector<double>       V;
  I.reserve(std::min(m, n));
  J.reserve(std::min(m, n));
  V.reserve(std::min(m, n));
  for (std::size_t i = 0; i < std::min(m, n); ++i) {
    I.push_back(i); J.push_back(i); V.push_back(1.0);
  }

  SpMV::SparseMatrix_DEN<double> A(m, n);
  A.assembleStorage(I, J, V, m, n);

  ASSERT(A.values().size() == m * n);
  ASSERT(A.values()[0 * n + 0] == 1.0);
  ASSERT(A.values()[100 * n + 100] == 1.0);
  ASSERT(A.values()[200 * n + 200] == 1.0);
  ASSERT(A.values()[0 * n + 1] == 0.0);
  ASSERT(A.values()[50 * n + 49] == 0.0);
  ASSERT(A.values()[255 * n + 0] == 0.0);

  for (double x : A.values()) ASSERT(!(std::isnan(x) || std::isinf(x)));

  std::cout << "[Pass] Large matrix smoke check\n";
}

// -----------------------------------------------------------------------------
// Allocation and deallocation
// Coverage: Task "Verify proper allocation/deallocation."
// -----------------------------------------------------------------------------
TEST_CASE(den_allocation_and_lifetime)
{
  for (int rep = 0; rep < 8; ++rep) {
    const std::size_t m = 8, n = 8;
    std::vector<std::size_t> I = {0, 3, 7};
    std::vector<std::size_t> J = {0, 4, 7};
    std::vector<double>       V = {1.0, 2.0, 3.0};

    SpMV::SparseMatrix_DEN<double> A(m, n);
    A.assembleStorage(I, J, V, m, n);
    ASSERT(A.values().size() == m * n);
  }

  std::cout << "[Pass] Allocation/deallocation behaved correctly\n";
}

// Suite runner
TEST_SUITE(DEN_storage_suite)
{
  TEST(den_requires_assembly_before_access);
  TEST(den_storage_dimensions_mn);
  TEST(den_index_bounds);
  TEST(den_values_match_and_zeros_clean);
  TEST(den_invalid_inputs_fail_safely);
  TEST(den_extremely_small_matrices);
  TEST(den_very_large_matrix_smoke);
  TEST(den_allocation_and_lifetime);
}

auto main() -> int
{
  RUN_SUITE(DEN_storage_suite);
  return 0;
}
