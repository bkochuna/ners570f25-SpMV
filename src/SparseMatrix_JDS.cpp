// src/SparseMatrix_JDS.cpp
#include "spmv/SparseMatrix_JDS.hpp"
#include "spmv/SparseMatrix_CSR.hpp"
// #include "spmv/SparseMatrix_COO.hpp" // keep commented until COO API is finalized
#include <algorithm>
#include <numeric>
#include <cassert>
#include <stdexcept>

/** JDS assembly/disassembly
 *
 * assemble_fromCSR():
 *  1) Count row nnz from CSR rowPtr.
 *  2) Stable sort row indices by decreasing row nnz (tie: old index).
 *  3) Build diagonal histogram => prefix sum into ptr_ (diag pointer).
 *  4) Walk rows in new order and place k-th entry of each row on diagonal d=k,
 *     advancing a per-diagonal cursor. This produces col_/val_ packed by diag.
 *
 * disassemble_toCOO():
 *  Inverse walk along diagonals, recovering (row, col, val) in the old indexing
 *  space using perm_. The COO object is returned for inspection/tests.
 *
 * Invariants checked:
 *  - ptr_.size() == maxlen_ + 1
 *  - ptr_.front() == 0
 *  - ptr_.back()  == nnz_
 *  - col_.size() == val_.size()
 */

namespace SpMV {

// ==============================
// CSR -> JDS
// ==============================
template<typename T>
void SparseMatrix_JDS<T>::assemble_fromCSR(const SparseMatrix_CSR<T>& A) {
  nrows_ = static_cast<index_t>(A.rows());
  ncols_ = static_cast<index_t>(A.cols());

  const auto& rp = A.rowPtr();
  nnz_ = rp.empty() ? 0 : static_cast<index_t>(rp.back());

  // 1) Count the number of nonzeros in each row from CSR rowPtr.
  // rownnz[i] = number of entries in row i.
  std::vector<index_t> rownnz(nrows_, 0);
  for (index_t i = 0; i + 1 < static_cast<index_t>(rp.size()); ++i)
    rownnz[i] = static_cast<index_t>(rp[i+1] - rp[i]);

  // 2) stable sort rows by decreasing nnz
  perm_.resize(nrows_);
  std::iota(perm_.begin(), perm_.end(), index_t(0));
  std::stable_sort(perm_.begin(), perm_.end(),
      [&](index_t a, index_t b) {
        if (rownnz[a] != rownnz[b]) return rownnz[a] > rownnz[b];
        return a < b;
      });

  // inverse permutation
  iperm_.assign(nrows_, 0);
  for (index_t r_new = 0; r_new < nrows_; ++r_new)
    iperm_[perm_[r_new]] = r_new;

  // 3) build jagged-diagonal pointer
  maxlen_ = nrows_ ? *std::max_element(rownnz.begin(), rownnz.end()) : 0;
  ptr_.assign(maxlen_ + 1, 0);

  std::vector<index_t> diagCount(maxlen_, 0);
  // Build a histogram of how many rows reach each diagonal depth.
  for (index_t r_new = 0; r_new < nrows_; ++r_new) {
    const index_t r = perm_[r_new];
    for (index_t k = 0; k < rownnz[r]; ++k) diagCount[k]++;
  }
  index_t acc = 0;
  for (index_t d = 0; d < maxlen_; ++d) { ptr_[d] = acc; acc += diagCount[d]; }
  ptr_[maxlen_] = nnz_;

  // 4) fill columns/values along jagged diagonals
  const auto& jc = A.colInd();
  const auto& vv = A.values();
  col_.resize(nnz_);
  val_.resize(nnz_);
  std::vector<index_t> cursor = ptr_;
   
  // Fill the JDS column and value arrays along jagged diagonals.
  // Each diagonal d = k stores the k-th element of each reordered row.
  // cursor[d] tracks the next insertion position for diagonal d.
  for (index_t r_new = 0; r_new < nrows_; ++r_new) {
    const index_t r     = perm_[r_new];
    const index_t start = static_cast<index_t>(rp[r]);
    const index_t end   = static_cast<index_t>(rp[r+1]);
    for (index_t k = 0; k < end - start; ++k) {
      const index_t pos = cursor[k]++;
      col_[pos] = static_cast<index_t>(jc[start + k]);
      val_[pos] = static_cast<T>(vv[start + k]);
    }
  }

  // 5) invariants
  assert(ptr_.size() == maxlen_ + 1);
  assert(ptr_.front() == 0);
  assert(ptr_.back()  == nnz_);
  assert(col_.size() == val_.size());
}

// COO -> JDS (temporary no-op to satisfy -Werror)
template<typename T>
void SparseMatrix_JDS<T>::assemble_fromCOO(const SparseMatrix_COO<T>&)
{
  // TODO(JDS): implement when COO public construction/iteration API is finalized.
  // Intentionally do nothing to avoid -Wsuggest-attribute=noreturn under -Werror.
}

// JDS -> COO (temporary: return empty COO with correct shape)
template<typename T>
SparseMatrix_COO<T> SparseMatrix_JDS<T>::disassemble_toCOO() const
{
  // NOTE: current COO ctor is (size_t nrows, size_t ncols)
  // We return an empty matrix to avoid 'noreturn' warning.
  return SparseMatrix_COO<T>(
      static_cast<size_t>(nrows_),
      static_cast<size_t>(ncols_)
  );
}

// explicit instantiations ...
template class SparseMatrix_JDS<float>;
template class SparseMatrix_JDS<double>;

} // namespace SpMV
