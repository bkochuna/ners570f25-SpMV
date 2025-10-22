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
        //SparseMatrix<fp_type>::setValue(i, j, val)
    {
        cout << "Hello from COO setValue" << endl;
        
        assert(i < this->_nrow);
        assert(j < this->_ncol);
        assert(this->_state == MatrixState::assembled);

        this->_state = MatrixState::initialized;
        
        // Check if the i,j are present in _buildCoeff
        // if yes, replace
        // if not, add
        // afterwards, reassemble
        // Actually this all could be replaced with a call to 
        // SparseMatrix<fp_type>::setValue(i, j, val)
        auto it = this->_buildCoeff.find(std::make_pair(i,j));
        if (it != this->_buildCoeff.end()) {
            it->second = val;
        } else {
            this->_buildCoeff[std::make_pair(i,j)] = val;
        }
        // Re-assemble the matrix
        assemble();
        assert(this->_state == MatrixState::assembled);
    }

    template <class fp_type>
    fp_type SparseMatrix_COO<fp_type>::getValue(const size_t i, const size_t j) :
        //SparseMatrix<fp_type>::getValue(i, j)
    {
        cout << "Hello from COO getValue" << endl;
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
}

// Need to declare the concrete templates within the library for
// use in code that links to libspmv
template class SpMV::SparseMatrix_COO<float>;
template class SpMV::SparseMatrix_COO<double>;
