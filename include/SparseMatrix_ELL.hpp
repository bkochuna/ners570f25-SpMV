#ifndef __SPMV_SparseMatrix_ELL__
#define __SPMV_SparseMatrix_ELL__

#include "SparseMatrix.hpp"
#include <vector>

namespace SpMV {
template <class fp_type> class SparseMatrix_ELL : public SparseMatrix<fp_type> {
protected:
  size_t _maxNnzPerRow = 0;
  std::vector<fp_type> _values{};
  std::vector<int> _colIndices{}; // signed to allow -1 for padding (if desired
                                  // by matVec implementation)

public:
  SparseMatrix_ELL(const size_t nrows, const size_t ncols);
  ~SparseMatrix_ELL();
  void assemble();
};
} // namespace SpMV

#endif