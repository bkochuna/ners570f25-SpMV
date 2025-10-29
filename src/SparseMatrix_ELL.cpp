#include "SparseMatrix_ELL.hpp"
#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

using std::pair;
using std::size_t;

namespace SpMV {

template <class fp_type>
SparseMatrix_ELL<fp_type>::SparseMatrix_ELL(const size_t nrows, const size_t ncols)
  : SparseMatrix<fp_type>(nrows, ncols) {
  std::cout << "Hello from SparseMatrix_ELL Constructor" << std::endl;
}

template <class fp_type>
SparseMatrix_ELL<fp_type>::~SparseMatrix_ELL() {
  std::cout << "Goodbye from SparseMatrix_ELL Destructor" << std::endl;
}

template <class fp_type>
void SparseMatrix_ELL<fp_type>::assemble() {

  std::cout << "Hello from SparseMatrix_ELL assemble" << std::endl;

  // Pull sizes/state from base
  const size_t nrows = this->_nrows;
  const size_t ncols = this->_ncols;
  (void)ncols;

  // If nothing was staged, produce empty views
  if (this->_buildCoeff.empty()) {
    _maxNnzPerRow = 0;
    _values.clear();
    _colIndices.clear();
    this->_state = MatrixState::assembled;
    return;
  }

  // 1) Count per row to compute K = max nnz per row
  std::vector<size_t> count(nrows, 0);
  for (auto const& kv : this->_buildCoeff) {
    const size_t i = kv.first.first;
    // Bounds were asserted in setValue
    ++count[i];
  }
  _maxNnzPerRow = 0;
  for (size_t i = 0; i < nrows; ++i) {
    if (count[i] > _maxNnzPerRow) _maxNnzPerRow = count[i];
  }

  // 2) Allocate flat ELL arrays (row-major)
  const size_t K = _maxNnzPerRow;
  _values.assign(nrows * K, fp_type{0});
  _colIndices.assign(nrows * K, -1);

  // 3) Bucket by row, then sort each row by column for deterministic layout
  std::vector<std::vector<pair<int, fp_type>>> buckets(nrows);
  buckets.reserve(nrows);
  for (auto const& kv : this->_buildCoeff) {
    const size_t i = kv.first.first;
    const int    j = static_cast<int>(kv.first.second);
    const fp_type v = kv.second;
    buckets[i].push_back({j, v});
  }
  for (size_t i = 0; i < nrows; ++i) {
    std::stable_sort(buckets[i].begin(), buckets[i].end(),
                     [](auto const& a, auto const& b){ return a.first < b.first; });
  }

  // 4) Pack into ELL (row-major, padding remains as (-1, 0))
  for (size_t i = 0; i < nrows; ++i) {
    size_t t = 0;
    for (auto const& cv : buckets[i]) {
      const size_t idx = i * K + t;
      _colIndices[idx] = cv.first;
      _values[idx]     = cv.second;
      ++t;
      if (t >= K) break; // safety
    }
  }

  // 5) Update base state/nnz
  this->_numnz = this->_buildCoeff.size();
  this->_state = MatrixState::assembled;
}

// Explicit instantiation
template class SparseMatrix_ELL<float>;
template class SparseMatrix_ELL<double>;

} // namespace SpMV
