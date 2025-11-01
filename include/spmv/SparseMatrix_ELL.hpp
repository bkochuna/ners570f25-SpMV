#ifndef __SPMV_SparseMatrix_ELL__
#define __SPMV_SparseMatrix_ELL__

#include "spmv/SparseMatrix.hpp"
#include <cstddef>
#include <vector>

namespace SpMV {

/**
 * ELL sparse matrix storage:
 * - Each row stores exactly _maxEntriesPerRow slots.
 * - Padded slots are marked by column index == _ncols (sentinel).
 */
template <class fp_type>
class SparseMatrix_ELL : public SparseMatrix<fp_type> {
public:
  // Construct an empty ELL matrix of size (nrows x ncols).
  SparseMatrix_ELL(std::size_t nrows, std::size_t ncols);
  ~SparseMatrix_ELL();

  // Build internal ELL storage from base builder (stub for now).
  void assemble();

  // y = A * x
  std::vector<fp_type> matvec(const std::vector<fp_type>& x) const;

private:
  // ELL storage: each row has exactly _maxEntriesPerRow slots
  // A padded slot is indicated by col == this->_ncols.
  std::vector<fp_type>     _values{};
  std::vector<std::size_t> _colIdx{};
  std::size_t              _maxEntriesPerRow{0};
};

} // namespace SpMV

#endif // __SPMV_SparseMatrix_ELL__
