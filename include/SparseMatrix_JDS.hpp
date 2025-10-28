#ifndef __SPMV_SparseMatrix_JDS__
#define __SPMV_SparseMatrix_JDS__

#include "SparseMatrix.hpp"
#include <vector>

namespace SpMV {

template <class fp_type>
class SparseMatrix_JDS : public SparseMatrix<fp_type> {
protected:
    // JDS format storage components
    std::vector<int> _perm{};           // Row permutation vector
    std::vector<int> _jd_ptr{};         // Starting index of each jagged diagonal
    std::vector<int> _colIndices{};     // Column indices for each value
    std::vector<fp_type> _values{};     // Non-zero values stored jagged-diagonally
    size_t _numDiagonals = 0;           // Number of diagonals in JDS format

public:
    // Constructor
    SparseMatrix_JDS(const size_t nrows, const size_t ncols);

    // Destructor
    ~SparseMatrix_JDS();

    // Assemble JDS from COO/CSR (optional to implement now)
    void assemble();
};

} // namespace SpMV

#endif // __SPMV_SparseMatrix_JDS__
