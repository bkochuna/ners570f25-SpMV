#ifndef __SPMV_SparseMatrix_ELL__
#define __SPMV_SparseMatrix_ELL__

#include "SparseMatrix.hpp"
#include <vector>

/**
 * ELLPACK (ELL) sparse matrix storage template declaration.
 *
 * Detailed description: Header declaring the ELL sparse matrix format. Inherits
 * from SparseMatrix. This format comes from ELLPACK, an elliptical PDE solver.
 * It is particularly efficient for SIMD instructions or with GPU
 * parallelization. The implementation should work at minimum for both `double`
 * and `float`, hence the templating via `fp_type` (floating point type)
 */

namespace SpMV {
template <class fp_type> class SparseMatrix_ELL : public SparseMatrix<fp_type> {
protected:
  /**
   * Maximum number of non-zero entries per row.
   */
  size_t _maxNnzPerRow = 0;

  /**
   * Value used to indicate padding in _colIndices{} for ELL matrix format.
   * Default of -1
   */
  int _paddingValue = -1; //

  /**
   * Flattened storage for non-zero values. Junk values where padding indicated.
   */
  std::vector<fp_type> _values{};

  /**
   * Column indices for each stored value.
   * Signed type allows using `paddingValue` (-1) to mark unused slots.
   */
  std::vector<int> _colIndices{};

public:
  /**
   * Construct an empty ELL matrix with the given size.
   *
   * @param nrows Number of rows in the matrix.
   * @param ncols Number of columns in the matrix.
   *
   * `_values` and `_colIndices` are empty and
   * `_maxNnzPerRow` is zero until `assemble()` is called.
   */
  SparseMatrix_ELL(const size_t nrows, const size_t ncols);

  /**
   * Destroy the matrix and free resources.
   */
  ~SparseMatrix_ELL();

  /**
   * Assemble the ELL matrix from the inherited COO-like
   * representation
   */
  void assemble();
};
} // namespace SpMV

#endif