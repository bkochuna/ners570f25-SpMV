#include "SparseMatrix_CSR.hpp"
#include <stdexcept>   // Definition of done: constructor verifies attributes (throw on invalid)
#include <limits>      // Definition of done: constructor verifies values are finite (if checked)

namespace SpMV
{
    // Definition of done: default constructor exists
    // Task: use initializer list to call base class (0x0)
    template <class fp_type>
    SparseMatrix_CSR<fp_type>::SparseMatrix_CSR()
    : SparseMatrix<fp_type>(0, 0)
    {}

    // Definition of done: assembling constructor (one arg per member) directly assembles the class
    // Task: use initializer list to set base and members
    // Definition of done: constructor verified to set all CSR attributes correctly (basic checks)
    template <class fp_type>
    SparseMatrix_CSR<fp_type>::SparseMatrix_CSR(const size_t nrows, const size_t ncols,
                                                std::vector<size_t> rowPtr,
                                                std::vector<size_t> colInd,
                                                std::vector<fp_type> values)
    : SparseMatrix<fp_type>(nrows, ncols),
      _rowPtr(std::move(rowPtr)),
      _colInd(std::move(colInd)),
      _values(std::move(values))
    {
        // Minimal, direct verification to satisfy “constructor verified…” in DoD
        const size_t m = this->nrows();
        const size_t n = this->ncols();

        if (_rowPtr.size() != m + 1)               throw std::invalid_argument("CSR: rowPtr size");
        if (_colInd.size() != _values.size())      throw std::invalid_argument("CSR: sizes mismatch");
        for (size_t r = 1; r < _rowPtr.size(); ++r)
            if (_rowPtr[r] < _rowPtr[r-1])         throw std::invalid_argument("CSR: rowPtr order");
        if (_rowPtr.back() != _values.size())      throw std::invalid_argument("CSR: rowPtr[m]!=nnz");
        for (auto c : _colInd) if (c >= n)         throw std::out_of_range("CSR: colInd OOB");
        for (auto v : _values)
            if (!std::isfinite(static_cast<double>(v))) throw std::invalid_argument("CSR: non-finite");
    }

    // Definition of done: destructor frees memory and nulls pointers
    template <class fp_type>
    SparseMatrix_CSR<fp_type>::~SparseMatrix_CSR()
    {
        _rowPtr.clear(); _colInd.clear(); _values.clear();
        _rowPtr.shrink_to_fit(); _colInd.shrink_to_fit(); _values.shrink_to_fit();
        // (Vectors own their storage; clearing releases memory. No raw pointers to null.)
    }

    // Task: function stubs were declared in the header; implementation will be added later by teammates
} // namespace SpMV



template class SpMV::SparseMatrix_CSR<float>;
template class SpMV::SparseMatrix_CSR<double>;
