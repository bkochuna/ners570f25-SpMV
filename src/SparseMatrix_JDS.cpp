#include "SparseMatrix_JDS.hpp"

#include <iostream>
#include <stddef.h>

using namespace std;

namespace SpMV
{
    template <class fp_type>
    SparseMatrix_JDS<fp_type>::SparseMatrix_JDS(const size_t nrows, const size_t ncols) :
        SparseMatrix<fp_type>::SparseMatrix(nrows, ncols)
    {
        cout << "Hello from JDS Constructor" << endl;
    }

    template <class fp_type>
    void SparseMatrix_JDS<fp_type>::assemble()
    {
        cout << "Hello from JDS assemble" << endl;

        // This routine will later convert _buildCoeff into JDS storage format.
    }

    template <class fp_type>
    void SparseMatrix_JDS<fp_type>::matvec(const std::vector<fp_type>& x, std::vector<fp_type>& y) const
    {
        cout << "Hello from JDS matvec" << endl;

        y.assign(this->_nrows, static_cast<fp_type>(0));

        // Perform y = A * x in JDS format
        for (size_t jd = 0; jd + 1 < jd_ptr.size(); ++jd)
        {
            size_t start = jd_ptr[jd];
            size_t end = jd_ptr[jd + 1];
            size_t diag_len = end - start;

            for (size_t i = 0; i < diag_len; ++i)
            {
                size_t row = perm[i];
                size_t col = col_idx[start + i];
                y[row] += val[start + i] * x[col];
            }
        }
    }
}


// Accessor method implementations
template <class fp_type>
const std::vector<fp_type>& SpMV::SparseMatrix_JDS<fp_type>::get_jdiag() const {
    return val.data();
}

template <class fp_type>
const std::vector<size_t>& SpMV::SparseMatrix_JDS<fp_type>::get_col_ind() const {
    return col_idx.data();
}

template <class fp_type>
const std::vector<size_t>& SpMV::SparseMatrix_JDS<fp_type>::get_perm() const {
    return perm.data();
}

template <class fp_type>
const std::vector<size_t>& SpMV::SparseMatrix_JDS<fp_type>::get_jd_ptr() const {
    return jd_ptr.data();
}

template <class fp_type>
size_t SpMV::SparseMatrix_JDS<fp_type>::get_nz() const {
    return val.size();
}

template <class fp_type>
size_t SpMV::SparseMatrix_JDS<fp_type>::get_ncol() const {
    return this->_ncols;
}

template <class fp_type>
size_t SpMV::SparseMatrix_JDS<fp_type>::get_nrow() const {
    return this->_nrows;
}

template <class fp_type>
size_t SpMV::SparseMatrix_JDS<fp_type>::get_maxnz_row() const { // Calculate the maximum number of non-zeroes per row
    if (jd_ptr.empty()) return 0;
    size_t max_len = 0;
    for (size_t i = 0; i < jd_ptr.size() - 1; i++) {
        size_t len = jd_ptr[i+1] - jd_ptr[i];
        if (len > max_len) max_len = len;
    }
    return max_len;
}


// Need to declare the concrete templates within the library
// for use in code that links to libspmv
template class SpMV::SparseMatrix_JDS<float>;
template class SpMV::SparseMatrix_JDS<double>;

