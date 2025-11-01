/** SparseMatrix_JDS.hpp
 *  Jagged Diagonal Storage (JDS) sparse matrix data structure.
 *
 *  Design goals:
 *   - Reorder rows by decreasing per-row nnz to improve memory locality.
 *   - Store entries along "jagged diagonals" to enable coalesced reads in SpMV.
 *   - Keep a stable tie-break to make the transform deterministic.
 *
 *  Conventions/assumptions:
 *   - 0-based indexing.
 *   - maxlen_ == maximum per-row nnz after reordering.
 *   - ptr_[d] is the starting offset of diagonal d (size = maxlen_ + 1).
 *   - col_[k], val_[k] store column indices and values aligned with diag offsets.
 *   - perm_ maps new row index -> old row index; iperm_ is the inverse.
 */

#ifndef SPMV_SPARSEMATRIX_JDS_HPP
#define SPMV_SPARSEMATRIX_JDS_HPP

#include <vector>
#include <cstddef>
#include "SparseMatrix_COO.hpp"
#include "SparseMatrix_CSR.hpp"

namespace SpMV {

    /** SparseMatrix_JDS
 *  fp_type value scalar (float or double).
 *  Encapsulates JDS storage and transformations from CSR/COO.
 *
 *  Public API mirrors other formats: assemble_fromCSR/COO, disassemble_toCOO.
 *  SpMV kernel itself can iterate along diagonals using ptr_ and maxlen_.
 */


template<typename T>
class SparseMatrix_JDS {
 public:
  using index_t = std::size_t;
  using value_t = T;

  void assemble_fromCSR(const SparseMatrix_CSR<T>& A);
  void assemble_fromCOO(const SparseMatrix_COO<T>& A);
  SparseMatrix_COO<T> disassemble_toCOO() const;

 private:
  index_t nrows_{0}, ncols_{0}, nnz_{0}, maxlen_{0};
  std::vector<index_t> perm_, iperm_, ptr_, col_;
  std::vector<value_t> val_;
};

} // namespace SpMV
#endif
