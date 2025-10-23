#ifndef __SPMV_SparseMatrix_CSR__
#define __SPMV_SparseMatrix_CSR__

#include "SparseMatrix.hpp"
#include <vector>
#include <cstddef>
#include <stdexcept>
#include <unordered_map>

namespace SpMV {

template <class fp_type>
class SparseMatrix_CSR : public SparseMatrix<fp_type>
{
public:
    using index_type = std::size_t;
    using value_type = fp_type;

    // states
    enum class State {Building, Assembled};

    // matvec function
    void matvec(const std::vector<fp_type>& x, std::vector<fp_type>& y);

private:
    // CSR storage (only valid in Assembled state)
    std::vector<index_type> _ia;  // size nrows+1
    std::vector<index_type> _ja;  // size nnz
    std::vector<value_type> _a;   // size nnz
};

} //namespace SpMV

#endif