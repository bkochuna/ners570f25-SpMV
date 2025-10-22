#ifndef __SPMV_SparseMatrix_COO__
#define __SPMV_SparseMatrix_COO__

#include "SparseMatrix.hpp"
#include <vector>
#include <cstddef>
#include <stdexcept>

namespace SpMV
{
    template <class fp_type>
    class SparseMatrix_COO : public SparseMatrix<fp_type>
    {
    public:
        using index_type = std::size_t;
        using value_type = fp_type;

        // ctor & finalize
        SparseMatrix_COO(const size_t nrows, const size_t ncols);
        void assemble(); // implemented in .cpp

        // --- minimal accessors ---
        inline index_type nrows() const noexcept { return this->_nrows; }
        inline index_type ncols() const noexcept { return this->_ncols; }
        inline index_type nnz()   const noexcept { return _row_index.size(); }

        inline index_type row(index_type k) const { return _row_index.at(k); }
        inline index_type col(index_type k) const { return _col_index.at(k); }
        inline value_type val(index_type k) const { return _values.at(k); }

        // --- insert entries prior to assemble() ---
        inline void insert(index_type i, index_type j, value_type v) {
            if (_assembled) {
                throw std::logic_error("COO insert after assemble()");
            }
            if (i >= this->_nrows || j >= this->_ncols) {
                throw std::out_of_range("COO insert index out of range");
            }
            _row_index.push_back(i);
            _col_index.push_back(j);
            _values.push_back(v);
        }

    private:
        std::vector<index_type> _row_index;
        std::vector<index_type> _col_index;
        std::vector<value_type> _values;
        bool _assembled = false;
    };
}

#endif
