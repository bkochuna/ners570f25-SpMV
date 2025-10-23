#include "SparseMatrix_CSR.hpp"
#include <algorithm>     // optional
#include <limits>
#include <iostream>
#include <stdexcept>

using namespace std;

namespace SpMV {

template <class fp_type>
void SparseMatrix_CSR<fp_type>::assemble()
{
    if (_state != State::Building)
        throw std::logic_error("assembleStorage(): state must be Building");
    const index_type R = this->_nrows;
    const index_type C = this->_ncols;

    // Allocate CSR row pointer (nrows+1), reset to zero counts
    _ia.assign(R + 1, 0);
    _ja.clear();
    _a.clear();

    // Edge case: empty matrix
    if (_triplets.empty()) {
        // _ia already zero-initialized; _ia[R]==0 == nnz
        _state = State::Assembled;
        return;
    }

    // ---- Approach: per-row unordered_map to sum duplicates in O(nnz) average time.
    // No sorting is required by the spec; columns will be in unspecified order.
    std::vector<std::unordered_map<index_type, value_type>> row_maps(R);

    for (const auto &t : _triplets) {
        // Bounds (defensive; insert already checks)
        if (t.i >= R || t.j >= C)
            throw std::out_of_range("assembleStorage(): triplet out of range");
        row_maps[t.i][t.j] += t.v;   // sum duplicates
    }

    // Row counts → prefix sum into _ia
    for (index_type i = 0; i < R; ++i)
        _ia[i + 1] = _ia[i] + static_cast<index_type>(row_maps[i].size());

    const index_type NNZ = _ia[R];
    _ja.resize(NNZ);
    _a.resize(NNZ);

    // Fill ja/a per row in row-major order.
    // (Columns not necessarily sorted; sort keys here if you want deterministic order.)
    for (index_type i = 0; i < R; ++i) {
        index_type k = _ia[i];
        if (!row_maps[i].empty()) {
            // Optional: enforce sorted columns for nicer invariants
            // std::vector<index_type> cols; cols.reserve(row_maps[i].size());
            // for (auto &kv : row_maps[i]) cols.push_back(kv.first);
            // std::sort(cols.begin(), cols.end());

            for (auto &kv : row_maps[i]) {
                _ja[k] = kv.first;
                _a[k]  = kv.second;
                ++k;
            }
        }
    }

    // Basic DBC: sizes & bounds
    if (_ia.front() != 0 || _ia.back() != NNZ)
        throw std::runtime_error("assembleStorage(): invalid row pointer bounds");
    if (_ja.size() != NNZ || _a.size() != NNZ)
        throw std::runtime_error("assembleStorage(): size mismatch in CSR arrays");

    // Clear builder and set state
    _triplets.clear();
    _triplets.shrink_to_fit();
    _state = State::Assembled;
}

template <class fp_type>
void SparseMatrix_CSR<fp_type>::disassembleStorage()
{
    if (_state != State::Assembled)
        throw std::logic_error("disassembleStorage(): state must be Assembled");

    const index_type R = this->_nrows;

    _triplets.clear();
    for (index_type i = 0; i < R; ++i) {
        const index_type beg = _ia[i];
        const index_type end = _ia[i + 1];
        for (index_type k = beg; k < end; ++k) {
            _triplets.push_back(Trip{i, _ja[k], _a[k]});
        }
    }

    // Free CSR arrays
    _ia.clear(); _ia.shrink_to_fit();
    _ja.clear(); _ja.shrink_to_fit();
    _a.clear();  _a.shrink_to_fit();

    _state = State::Building;
}

// Explicit template instantiations used by the project
template class SparseMatrix_CSR<double>;
template class SparseMatrix_CSR<float>;

} // namespace SpMV
