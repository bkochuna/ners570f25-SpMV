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

// Need to declare the concrete templates within the library
// for use in code that links to libspmv
template class SpMV::SparseMatrix_JDS<float>;
template class SpMV::SparseMatrix_JDS<double>;

