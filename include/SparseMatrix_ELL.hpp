#ifndef __SPMV_SparseMatrix_ELL__
#define __SPMV_SparseMatrix_ELL__

#include "SparseMatrix.hpp"
#include <vector>

namespace SpMV {
template <class fp_type> class SparseMatrix_ELL : public SparseMatrix<fp_type> {
protected:
  size_t _maxEntriesPerRow = 0;
  std::vector<fp_type> _values{};
  std::vector<std::size_t> _colIdx{}; // entries equal to _ncols mark padded slots

public:
  SparseMatrix_ELL(const size_t nrows, const size_t ncols);
  ~SparseMatrix_ELL();
  void assemble();
  std::vector<fp_type> matvec(const std::vector<fp_type> &x) const;
};
} // namespace SpMV

#endif