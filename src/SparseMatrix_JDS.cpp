#include "SparseMatrix_JDS.hpp"

#include <iostream>
#include <stddef.h>
#include <stdexcept>
#include <cassert>

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
        //cout << "Hello from JDS assemble" << endl;

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
// Set and Get value
    template <class fp_type>
    fp_type SpMV::SparseMatrix_JDS<fp_type>::getValue(const size_t i, const size_t j)
    {
	assert(this->_state >= MatrixState::initialized);
        
        // Check bounds
        if (i >= this->_nrows || j >= this->_ncols) {
            throw std::out_of_range("Matrix indices out of range");
        }

        // During building phase, use base class implementation (std::map)
        if (this->_state == MatrixState::building) {
            return SparseMatrix<fp_type>::getValue(i, j);
        }
        
        // After assembly, search JDS format (this is inefficient but works)
        assert(this->_state == MatrixState::assembled);
        
        // Search through JDS arrays for the element (i, j)
        for (size_t idx = 0; idx < val.size(); ++idx) {
            // Find which diagonal this element belongs to
            size_t diag_idx = 0;
            while (diag_idx < jd_ptr.size() - 1 && idx >= jd_ptr[diag_idx + 1]) {
                diag_idx++;
            }
            
            // Calculate position within diagonal
            size_t pos_in_diag = idx - jd_ptr[diag_idx];
            
            // Check if this is our element
            if (pos_in_diag < perm.size() && 
                perm[pos_in_diag] == i && 
                col_idx[idx] == j) {
                return val[idx];
            }
        }
        
        // Element not found, return zero for sparse matrix
        return static_cast<fp_type>(0);
    }

    template <class fp_type>
    void SpMV::SparseMatrix_JDS<fp_type>::setValue(const size_t i, const size_t j, fp_type value)
    {
        assert(this->_state >= MatrixState::initialized);
        
        // Check bounds
        if (i >= this->_nrows || j >= this->_ncols) {
            throw std::out_of_range("Matrix indices out of range");
        }

        // If we're already assembled, setting a value should revert to building state
        if (this->_state == MatrixState::assembled) {
            // Reset and use base class behavior
            this->_state = MatrixState::building;
            // Clear JDS arrays since they're no longer valid
            val.clear();
            col_idx.clear();
            perm.clear();
            jd_ptr.clear();
            this->_numnz = 0;
        }
        
        // Use base class implementation for building phase
        SparseMatrix<fp_type>::setValue(i, j, value);
    }
}




// Returns pointer to the non-zero values array
template <class fp_type>
const fp_type* SpMV::SparseMatrix_JDS<fp_type>::get_jdiag() const {
    return val.data();
}

// Returns pointer to the column indices array
template <class fp_type>
const size_t* SpMV::SparseMatrix_JDS<fp_type>::get_col_ind() const {
    return col_idx.data();
}

// Returns pointer to the permutation array
template <class fp_type>
const size_t* SpMV::SparseMatrix_JDS<fp_type>::get_perm() const {
    return perm.data();
}

// Returns pointer to the jagged diagonal pointers array
template <class fp_type>
const size_t* SpMV::SparseMatrix_JDS<fp_type>::get_jd_ptr() const {
    return jd_ptr.data();
}

// Returns total number of non-zero elements
template <class fp_type>
size_t SpMV::SparseMatrix_JDS<fp_type>::get_nz() const {
    return val.size();
}

// Returns number of columns
template <class fp_type>
size_t SpMV::SparseMatrix_JDS<fp_type>::get_ncol() const {
    return this->_ncols;
}

// Returns number of rows
template <class fp_type>
size_t SpMV::SparseMatrix_JDS<fp_type>::get_nrow() const {
    return this->_nrows;
}

// Calculate and returns the maximum number of non-zeroes per row
template <class fp_type>
size_t SpMV::SparseMatrix_JDS<fp_type>::get_maxnz_row() const {
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

