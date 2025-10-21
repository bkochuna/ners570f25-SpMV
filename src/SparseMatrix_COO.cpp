#include "SparseMatrix_COO.hpp"

#include <iostream>
#include <stddef.h>

using namespace std;

namespace SpMV
{
    template <class fp_type>
    SparseMatrix_COO<fp_type>::SparseMatrix_COO(const size_t nrows, const size_t ncols) :
        SparseMatrix<fp_type>::SparseMatrix(nrows, ncols)
    {
        cout << "Hello from COO Constructor" << endl;
    }

    template <class fp_type>
    void SparseMatrix_COO<fp_type>::assemble()
    {
        cout << "Hello from COO assemble" << endl;

        //This routine needs to convert _buildCoeff into the COO storage format.
    }

    template <class fp_type>
    void SparseMatrix_COO<fp_type>::setValue(const size_t i, const size_t j, fp_type val) :
        SparseMatrix<fp_type>::setValue(i, j, val)
    {
        cout << "Hello from COO setValue" << endl;
    }

    template <class fp_type>
    fp_type SparseMatrix_COO<fp_type>::getValue(const size_t i, const size_t j) :
        SparseMatrix<fp_type>::getValue(i, j)
    {
        cout << "Hello from COO getValue" << endl;
    }
}

// Need to declare the concrete templates within the library for
// use in code that links to libspmv
template class SpMV::SparseMatrix_COO<float>;
template class SpMV::SparseMatrix_COO<double>;
