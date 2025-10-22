#ifndef __SPMV_SparseMatrix_ELL__
#define __SPMV_SparseMatrix_ELL__

#include "SparseMatrix.hpp"

namespace SpMV {
template <class fp_type> class SparseMatrix_ELL : public SparseMatrix<fp_type> {
public:
  SparseMatrix_ELL(const size_t nrows, const size_t ncols);
  ~SparseMatrix_ELL();
  void assemble();
};
} // namespace SpMV

#endif