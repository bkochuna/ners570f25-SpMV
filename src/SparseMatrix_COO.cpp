#include "SparseMatrix_COO.hpp"

#include <iostream>
#include <stddef.h>
#include <algorithm>
#include <vector>
#include <cmath>
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
    void SparseMatrix_COO<fp_type>::view()
    {
        // view() should only be called by a matrix in the assembled state. Check to see if this is true. 
        assert(this->_state == MatrixState::assembled);

        // if there are no elements, output that and end method.
        if (this->_idx_row.empty() || this->_idx_col.empty() || this->_aij.empty())
        {
            cout << "No Elements in Matrix " << endl;
            return;
        }

        //find the largest elements in the row and column index arrays
        typename std::vector<size_t>::iterator max_in_rows= std::max_element(this->_idx_row.begin(), this->_idx_row.end());
        typename std::vector<size_t>::iterator max_in_cols= std::max_element(this->_idx_col.begin(), this->_idx_col.end());

        //find the number of digits in those largest indicies using the whole number of log10(index)+1
        int max_i =static_cast<int> (std::floor(std::log10(*max_in_rows)+1));
        int max_j =static_cast<int> (std::floor(std::log10(*max_in_cols)+1)); 
        //establish the required widths to print out 
        int i_min = 9; //characters in "Row Index"
        int j_min = 12; //characters in "Column Index"
        int i_width = std::max(max_i, i_min); //if one of the indicies is larger than the header column, 
        int j_width = std::max(max_j, j_min); //extra spaces must be added to keep columns aligned

        //print out the header;
        cout << "This is a matrix stored in the COO format. ";
        cout << "It has " << this->_nrows << " rows, " ;
        cout << this->_ncols << " columns, and " ;
        cout << this->_numnz << " non-zero values." << endl;

        cout << "Row Index";
        cout.width(i_width-i_min +3);
        cout << right << " | ";
        cout << "Column Index";
        cout.width(j_width-j_min +3);
        cout << right << " | ";
        cout << left << "Value" << endl;

        // print the values
        for(size_t i=0; i< this->_numnz; i++)
        {
            cout.width(i_width);
            cout << left << this->_idx_row[i] << " | ";
            cout.width(j_width);
            cout << left << this->_idx_col[i] << " | ";
            cout << this->_aij[i] << endl;
        }
    }
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
