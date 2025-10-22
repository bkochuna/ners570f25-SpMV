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
        assert(this->_state >= MatrixState::initialized);

        //print out the header;
        cout << "This is a matrix stored in the COO format. ";
        cout << "It has " << this->_nrows << " rows, " ;
        cout << this->_ncols << " columns, and " ;
        cout << this->_numnz << " non-zero values." << endl;


        if (this->_idx_row.empty() || this->_idx_col.empty() || this->_aij.empty())
        {
            cout << "No Elements in Matrix " << endl;
            return;
        }

        // find the largest elements in the row and column index arrays
        typename std::vector<size_t>::iterator max_in_rows= std::max_element(this->_idx_row.begin(), this->_idx_row.end());
        typename std::vector<size_t>::iterator max_in_cols= std::max_element(this->_idx_col.begin(), this->_idx_col.end());

        //find the length of those largest indicies
        int max_i =static_cast<int> (std::floor(std::log10(*max_in_rows)+1));
        int max_j =static_cast<int> (std::floor(std::log10(*max_in_cols)+1));
        //cout << max_i << ", " << max_j << endl;

        //establish the required widths to print out 
        int i_min = 9;
        int j_min = 12;
        int i_width = std::max(max_i, i_min);
        int j_width = std::max(max_j, j_min);
        //cout << i_width << ", " << j_width << endl;

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
}

// Need to declare the concrete templates within the library for
// use in code that links to libspmv
template class SpMV::SparseMatrix_COO<float>;
template class SpMV::SparseMatrix_COO<double>;
