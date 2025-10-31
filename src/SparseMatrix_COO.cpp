#include "SparseMatrix_COO.hpp"

#include <iostream>
#include <stddef.h>
#include <cassert>

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

    template <class fp_type>
    fp_type SparseMatrix_COO<fp_type>::getValue(const size_t i, const size_t j) :
    {
        assert(this->_state >= MatrixState::assembled);
        assert(i < this->_nrow);
        assert(j < this->_ncol);
        
        // Traverse the nz values and find the value at (i,j)
        for (size_t idx = 0; idx < this->_numnz; idx++)
        {
            if (this->_idx_row[idx] == i && this->_idx_col[idx] == j)
            {
                return this->_aij[idx];
            }
        }
        return 0.0;
    }

    template <class fp_type>
    std::vector<fp_type> SparseMatrix_COO<fp_type>::matvec(std::vector<fp_type> x) 
    {
        cout << "Hello from COO matvec" << endl;

        //This routine implements matvec for the COO format.

        //Make sure matrix is assembled
        assert( this->_state == assembled );

        //Input size must match column dimension of matrix
        assert( this->_ncols == x.size() );

        //Initialize return vector b with all zeros.
        std::vector<fp_type> b( this->_nrows, 0.);

        //For loop to perform matvec
        for (size_t i=0; i < (this->_numnz) ; i++) {
            b[this->_idx_row[i]] += this->_aij[i] * x[this->_idx_col[i]]; 
        }

        return b;

    }
}

// Need to declare the concrete templates within the library for
// use in code that links to libspmv
template class SpMV::SparseMatrix_COO<float>;
template class SpMV::SparseMatrix_COO<double>;
// this code redo for pull req
