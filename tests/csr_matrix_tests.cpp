#include <SpMV.hpp>

#include <vector>

#include "unit_test_framework.hpp"

namespace {

template <typename fp_type>
TEST_CASE(csr_construct_valid_basic) {
  // happy path: ensure constructor accepts well-formed CSR data and exposes invariants
  std::vector<size_t> rowPtr{0, 2, 3, 4};
  std::vector<size_t> colInd{0, 2, 1, 2};
  std::vector<fp_type> values{fp_type(1), fp_type(2), fp_type(3), fp_type(4)};

  SpMV::SparseMatrix_CSR<fp_type> csr(3, 3, rowPtr, colInd, values);

  ASSERT(csr.rows() == 3);
  ASSERT(csr.cols() == 3);

  const auto &rp = csr.rowPtr();
  const auto &ci = csr.colInd();
  const auto &vals = csr.values();

  ASSERT(rp.size() == csr.rows() + 1);
  for (size_t i = 0; i + 1 < rp.size(); ++i) {
    ASSERT(rp[i] <= rp[i + 1]);
  }
  ASSERT(rp.back() == vals.size());
  ASSERT(ci.size() == vals.size());
  for (auto col : ci) {
    ASSERT(col < csr.cols());
  }
}

template <typename fp_type>
TEST_CASE(csr_construct_empty) {
  // edge case: zero-by-zero matrix should still populate rowPtr with sentinel entry
  std::vector<size_t> rowPtr{0};
  std::vector<size_t> colInd;
  std::vector<fp_type> values;

  SpMV::SparseMatrix_CSR<fp_type> csr(0, 0, rowPtr, colInd, values);

  ASSERT(csr.rows() == 0);
  ASSERT(csr.cols() == 0);
  ASSERT(csr.rowPtr().size() == 1);
  ASSERT(csr.colInd().empty());
  ASSERT(csr.values().empty());
}

template <typename fp_type>
TEST_CASE(csr_construct_zero_rows) {
  // structural edge: allow rows with zero stored entries while maintaining CSR widths
  std::vector<size_t> rowPtr{0, 1, 1, 2, 2};
  std::vector<size_t> colInd{0, 2};
  std::vector<fp_type> values{fp_type(5), fp_type(6)};

  SpMV::SparseMatrix_CSR<fp_type> csr(4, 3, rowPtr, colInd, values);

  ASSERT(csr.rows() == 4);
  ASSERT(csr.cols() == 3);
  ASSERT(csr.rowPtr().size() == 5);
  ASSERT(csr.rowPtr().back() == csr.values().size());
}

template <typename fp_type>
TEST_CASE(csr_construction_destruction_cycles) {
  // repeat construction in a loop to shake out destructor memory handling assumptions
  std::vector<size_t> baseRowPtr{0, 1, 2};
  std::vector<size_t> baseColInd{0, 1};
  std::vector<fp_type> baseValues{fp_type(1), fp_type(2)};

  for (int iter = 0; iter < 256; ++iter) {
    SpMV::SparseMatrix_CSR<fp_type> csr(2, 2, baseRowPtr, baseColInd, baseValues);
    ASSERT(csr.rowPtr().size() == 3);
  }
}

template <typename fp_type>
TEST_CASE(csr_invalid_size_mismatch) {
  // guard: mismatch between value and column arrays should trigger a throw
  std::vector<size_t> rowPtr{0, 1, 2};
  std::vector<size_t> colInd{0, 1};
  std::vector<fp_type> values{fp_type(1)}; // size mismatch

  bool threw = false;
  try {
    SpMV::SparseMatrix_CSR<fp_type> csr(2, 2, rowPtr, colInd, values);
    (void)csr;
  } catch (...) {
    threw = true;
  }
  ASSERT(threw);
}

template <typename fp_type>
TEST_CASE(csr_invalid_rowptr_monotonic) {
  // guard: rowPtr must be nondecreasing across rows
  std::vector<size_t> rowPtr{0, 2, 1};
  std::vector<size_t> colInd{0, 1};
  std::vector<fp_type> values{fp_type(1), fp_type(2)};

  bool threw = false;
  try {
    SpMV::SparseMatrix_CSR<fp_type> csr(2, 2, rowPtr, colInd, values);
    (void)csr;
  } catch (...) {
    threw = true;
  }
  ASSERT(threw);
}

template <typename fp_type>
TEST_CASE(csr_invalid_rowptr_final_entry) {
  // guard: final rowPtr entry must match nnz count
  std::vector<size_t> rowPtr{0, 1, 3};
  std::vector<size_t> colInd{0, 1};
  std::vector<fp_type> values{fp_type(1), fp_type(2)};

  bool threw = false;
  try {
    SpMV::SparseMatrix_CSR<fp_type> csr(2, 2, rowPtr, colInd, values);
    (void)csr;
  } catch (...) {
    threw = true;
  }
  ASSERT(threw);
}

template <typename fp_type>
TEST_CASE(csr_invalid_column_index) {
  // guard: column indices must stay within matrix bounds at construction time
  std::vector<size_t> rowPtr{0, 1, 2};
  std::vector<size_t> colInd{0, 2}; // index 2 is out of bounds for ncols = 2
  std::vector<fp_type> values{fp_type(1), fp_type(2)};

  bool threw = false;
  try {
    SpMV::SparseMatrix_CSR<fp_type> csr(2, 2, rowPtr, colInd, values);
    (void)csr;
  } catch (...) {
    threw = true;
  }
  ASSERT(threw);
}

} // namespace

template <typename fp_type>
TEST_SUITE(csr_matrix_suite) {
  TEST((csr_construct_valid_basic<fp_type>));
  TEST((csr_construct_empty<fp_type>));
  TEST((csr_construct_zero_rows<fp_type>));
  TEST((csr_construction_destruction_cycles<fp_type>));
  TEST((csr_invalid_size_mismatch<fp_type>));
  TEST((csr_invalid_rowptr_monotonic<fp_type>));
  TEST((csr_invalid_rowptr_final_entry<fp_type>));
  TEST((csr_invalid_column_index<fp_type>));
}

auto main() -> int {
  RUN_SUITE((csr_matrix_suite<float>));
  RUN_SUITE((csr_matrix_suite<double>));
  return 0;
}
