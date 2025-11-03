#ifndef __SPMV_SparseMatrix_ELL__
#define __SPMV_SparseMatrix_ELL__

#include "SparseMatrix.hpp"
#include <vector>

namespace SpMV {
template <class fp_type> class SparseMatrix_ELL : public SparseMatrix<fp_type> {
protected:
  size_t _maxNnzPerRow = 0;
  int paddingValue = -1; // Value to use to indicate padding in column indices
  std::vector<fp_type> _values{};
  std::vector<int> _colIndices{}; // signed to allow -1 for padding (if desired
                                  // by matVec implementation)

public:
  SparseMatrix_ELL(const size_t nrows, const size_t ncols);
  ~SparseMatrix_ELL();
  void assemble();

  // --- Accessors (read-only) ---
  const std::vector<int>& getColInd() const;
  const std::vector<fp_type>& getValues() const;
  size_t getMaxNnzPerRow() const;
};
} // namespace SpMV

#endif
