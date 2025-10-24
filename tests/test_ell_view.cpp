#include <SpMV.hpp>
#include "SparseMatrix_ELL.hpp"
#include <vector>
#include <algorithm>
#include <cmath>
#include "unit_test_framework.hpp"

namespace {
constexpr double EPS = 1e-12;

struct Entry {
  int c;
  double v;
};

// Collect (col,val) entries for row r, ignoring padded slots (e.g., col == -1)
static std::vector<Entry>
row_entries(const std::vector<int>& cols, const std::vector<double>& vals,
            size_t r, size_t K)
{
  std::vector<Entry> out;
  out.reserve(K);
  for (size_t t = 0; t < K; ++t) {
    const size_t idx = r * K + t;
    const int c = cols[idx];
    if (c >= 0) { // ignore padding
      out.push_back({c, vals[idx]});
    }
  }
  // sort by column to compare deterministically
  std::sort(out.begin(), out.end(), [](const Entry& a, const Entry& b){
    return a.c < b.c;
  });
  return out;
}

// Compare two (col,val) lists after sorting by col; values compared with EPS
static void assert_same_entries(const std::vector<Entry>& got,
                                const std::vector<Entry>& exp)
{
  ASSERT(got.size() == exp.size());
  for (size_t k = 0; k < got.size(); ++k) {
    ASSERT(got[k].c == exp[k].c);
    ASSERT(std::fabs(got[k].v - exp[k].v) <= EPS);
  }
}
} // namespace

TEST_CASE(ell_view_shape_and_content_flat)
{
  const size_t nrows = 3, ncols = 3;
  SpMV::SparseMatrix_ELL<double> A(nrows, ncols);

  // Matrix:
  // [1 0 2]
  // [0 3 0]
  // [4 0 5]
  A.setValue(0, 0, 1.0);
  A.setValue(0, 2, 2.0);
  A.setValue(1, 1, 3.0);
  A.setValue(2, 0, 4.0);
  A.setValue(2, 2, 5.0);

  A.assemble();

  // Views
  const auto& vals = A.values_view();           // flat row-major length = nrows*K
  const auto& cols = A.col_indices_view();

  // Guards to avoid segfault if assemble() didn't allocate yet
  ASSERT(!cols.empty());
  ASSERT(!vals.empty());

  // Prefer row_width(); if it's 0 (or not reliable), infer K from sizes
  size_t K = A.row_width();
  if (K == 0) {
    ASSERT(cols.size() % nrows == 0);
    ASSERT(vals.size() % nrows == 0);
    K = cols.size() / nrows;
  }
  ASSERT(K > 0);
  ASSERT(vals.size() == nrows * K);
  ASSERT(cols.size() == nrows * K);

  // Expected per row (order-agnostic; we sort by col before comparing)
  // r=0: (0,1), (2,2)
  // r=1: (1,3)
  // r=2: (0,4), (2,5)
  {
    auto got0 = row_entries(cols, vals, 0, K);
    std::vector<Entry> exp0{{0,1.0},{2,2.0}};
    std::sort(exp0.begin(), exp0.end(), [](auto&a, auto&b){return a.c<b.c;});
    assert_same_entries(got0, exp0);
  }
  {
    auto got1 = row_entries(cols, vals, 1, K);
    std::vector<Entry> exp1{{1,3.0}};
    assert_same_entries(got1, exp1);
  }
  {
    auto got2 = row_entries(cols, vals, 2, K);
    std::vector<Entry> exp2{{0,4.0},{2,5.0}};
    std::sort(exp2.begin(), exp2.end(), [](auto&a, auto&b){return a.c<b.c;});
    assert_same_entries(got2, exp2);
  }
}

TEST_CASE(ell_view_const_correctness)
{
  SpMV::SparseMatrix_ELL<double> A(2, 3);
  A.setValue(0, 1, 7.0);
  A.assemble();

  const auto& C = A; // const reference must still allow read-only views
  (void)C.values_view();
  (void)C.col_indices_view();
  (void)C.row_width();
}

TEST_SUITE(ell_view_suite)
{
  TEST(ell_view_shape_and_content_flat);
  TEST(ell_view_const_correctness);
}

auto main() -> int
{
  RUN_SUITE(ell_view_suite);
  return 0;
}
