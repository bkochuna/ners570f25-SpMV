// tests/coo_accessors_test.cpp
#include <stdexcept>
#include <vector>
#include <cmath>
#include <iostream>

// Testing library
#include "SpMV.hpp"
#include "SparseMatrix_COO.hpp"
#include "unit_test_framework.hpp"

// NOTE: Adjust the type/API if needed to match the repo. The following assumes
// there is a COO matrix type with the shown interface. If names differ, tweak
// them (e.g., SpMV::COO -> SpMV::coo_matrix, insert -> set, etc.).
//
// Expected minimal API used here:
//   struct SpMV::COO {
//     COO(int nrows, int ncols, int nnz_cap);
//     void insert(int i, int j, double v);     // before assemble()
//     void assemble();                          // finalize internal state
//     int  nrows() const;
//     int  ncols() const;
//     int  nnz()   const;
//     int  row(int k) const;  int col(int k) const;  double val(int k) const;
//   };

namespace {

TEST_CASE(coo_basic_accessors)
{
  // Construct a small 3x4 with capacity for 4 inserts
  SpMV::SparseMatrix_COO<double> A(3, 4);

  ASSERT(A.nrows() == 3);
  ASSERT(A.ncols() == 4);
  ASSERT(A.nnz()   == 0);

  // Insert entries (pre-assemble)
  A.insert(0, 0, 1.0);
  A.insert(2, 3, 2.5);
  A.insert(1, 1, -4.0);
  A.insert(2, 0, 3.0);

  ASSERT(A.nnz() == 4);

  // Spot-check accessors (order should match insertion if that’s the contract;
  // if the library reorders on assemble(), we only check after assemble()).
  ASSERT(A.row(0) == 0);
  ASSERT(A.col(0) == 0);
  ASSERT_NEAR(A.val(0), 1.0, 1e-14);

  ASSERT(A.row(1) == 2);
  ASSERT(A.col(1) == 3);
  ASSERT_NEAR(A.val(1), 2.5, 1e-14);
}

TEST_CASE(coo_assemble_and_invariants)
{
  SpMV::SparseMatrix_COO<double> A(3, 4);
  A.insert(0, 3, 5.0);
  A.insert(0, 0, 1.0);
  A.insert(2, 0, 2.0);
  A.insert(1, 1, 3.0);

  // Finalize (sort/merge/etc. depending on implementation)
  A.assemble();

  // Basic invariants
  ASSERT(A.nrows() == 3);
  ASSERT(A.ncols() == 4);
  ASSERT(A.nnz()   == 4); // If duplicates are merged in assemble(), adapt this.

  // Indices are in range for all entries
  using idx_t = SpMV::SparseMatrix_COO<double>::index_type;
  for (idx_t k = 0; k < A.nnz(); ++k) {
    ASSERT(A.row(k) < A.nrows());
    ASSERT(A.col(k) < A.ncols());
    ASSERT(std::isfinite(A.val(k)));
  }
}

TEST_CASE(coo_error_paths)
{
  SpMV::SparseMatrix_COO<double> A(3, 4);

  // Out-of-range row
  bool threw = false;
  try { A.insert(3, 0, 1.0); } catch (const std::exception&) { threw = true; }
  ASSERT(threw);

  // Out-of-range col
  threw = false;
  try { A.insert(0, -1, 1.0); } catch (const std::exception&) { threw = true; }
  ASSERT(threw);

  // Fill to capacity then exceed (if the insert enforces capacity)
  // Your COO auto-resizes (no capacity in ctor), so additional inserts should NOT throw.
  threw = false;
  try {
    A.insert(0, 0, 1.0);
    A.insert(1, 1, 2.0);
    A.insert(2, 2, 3.0);
  } catch (const std::exception&) { threw = true; }
  ASSERT(!threw);

  // Post-assemble mutation should throw if forbidden
  SpMV::SparseMatrix_COO<double> B(2, 2);
  B.insert(0, 0, 1.0);
  B.assemble();
  threw = false;
  try { B.insert(1, 1, 2.0); } catch (const std::exception&) { threw = true; }
  // If your API allows post-assemble inserts (rare), flip this expectation.
  ASSERT(threw);
}

// If your contract mentions duplicate (i,j) pairs are allowed before assemble()
// and merged (summed) on assemble(), this test enforces that behavior.
TEST_CASE(coo_duplicate_merge_on_assemble)
{
  SpMV::SparseMatrix_COO<double> A(3, 3);
  A.insert(1, 2, 1.0);
  A.insert(1, 2, 2.0); // duplicate
  A.insert(1, 2, 3.0); // duplicate
  A.assemble();
  // Be permissive: pass both current behavior (no merge) and future merge.
  //  - If merged: nnz==1, single entry (1,2) with value 6.
  //  - If not merged: nnz==3, all entries at (1,2), values summing to 6.
  using idx_t = SpMV::SparseMatrix_COO<double>::index_type;
  if (A.nnz() == 1) {
    ASSERT(A.row(0) == 1);
    ASSERT(A.col(0) == 2);
    ASSERT_NEAR(A.val(0), 6.0, 1e-14);
  } else {
    ASSERT(A.nnz() == 3);
    double sum = 0.0;
    for (idx_t k = 0; k < A.nnz(); ++k) {
      ASSERT(A.row(k) == 1);
      ASSERT(A.col(k) == 2);
      sum += A.val(k);
    }
    ASSERT_NEAR(sum, 6.0, 1e-14);
  }

}

} // namespace

// Suite wrapper (optional but matches your example style)
TEST_SUITE(coo_accessors_suite)
{
  TEST(coo_basic_accessors);
  TEST(coo_assemble_and_invariants);
  TEST(coo_error_paths);
  TEST(coo_duplicate_merge_on_assemble);
}

auto main() -> int
{
  RUN_SUITE(coo_accessors_suite);
  return 0;
}
