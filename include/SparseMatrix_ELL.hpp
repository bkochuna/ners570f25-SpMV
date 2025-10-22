#ifndef __SPMV_SparseMatrix_ELL__
#define __SPMV_SparseMatrix_ELL__

#include "SparseMatrix.hpp"
#include "SparseMatrix_CSR.hpp"

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
  void assembleStorage(
        const std::vector<size_t>& i_idx,
        const std::vector<size_t>& j_idx,
        const std::vector<fp_type>& vals,
        size_t nrows,
        size_t ncols
    );
};
} // namespace SpMV

#endif