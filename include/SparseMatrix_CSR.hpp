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

    // States
    enum class State { Building, Assembled };

    // ---- Lifecycle ----
    SparseMatrix_CSR(index_type nrows, index_type ncols)
    : SparseMatrix<fp_type>(nrows, ncols), _state(State::Building)
    {}

    // Builder API (available only in Building state)
    inline void insert(index_type i, index_type j, value_type v) {
        if (_state != State::Building)
            throw std::logic_error("CSR insert: matrix is assembled");
        if (i >= this->_nrows || j >= this->_ncols)
            throw std::out_of_range("CSR insert: index out of range");
        _triplets.emplace_back(Trip{i, j, v});
    }

    // ---- Assembly / Disassembly ----
    // Build CSR arrays from collected triplets; sum duplicates.
    // Preconditions: state == Building
    // Postconditions: _ia.size()==nrows+1; _ia[0]==0; _ia[nrows]==nnz; state==Assembled; builder cleared.
    void assembleStorage();

    // Restore builder triplets from CSR arrays and free CSR storage.
    // Preconditions: state == Assembled
    // Postconditions: state==Building; CSR vectors empty; builder triplets restored.
    void disassembleStorage();

    // ---- Accessors (read-only) ----
    inline State state() const noexcept { return _state; }
    inline index_type nrows() const noexcept { return this->_nrows; }
    inline index_type ncols() const noexcept { return this->_ncols; }
    inline index_type nnz()   const noexcept { return _ja.size(); }

    inline const std::vector<index_type>& ia() const noexcept { return _ia; }  // row_ptr
    inline const std::vector<index_type>& ja() const noexcept { return _ja; }  // col_idx
    inline const std::vector<value_type>& a()  const noexcept { return _a; }   // values

    // matvec function
    void matvec(const std::vector<fp_type>& x, std::vector<fp_type>& y);

private:
    struct Trip { index_type i, j; value_type v; };

    // Builder container (only valid in Building state)
    std::vector<Trip> _triplets;

    // CSR storage (only valid in Assembled state)
    std::vector<index_type> _ia;  // size nrows+1
    std::vector<index_type> _ja;  // size nnz
    std::vector<value_type> _a;   // size nnz

    State _state;
};

} // namespace SpMV

#endif
