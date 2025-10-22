#include "SparseMatrix_DEN.hpp"

#include <iostream>
#include <stddef.h>

using namespace std;

namespace SpMV
{
    template <class fp_type>
    SparseMatrix_DEN<fp_type>::SparseMatrix_DEN(const size_t nrows, const size_t ncols) :  
        SparseMatrix<fp_type>::SparseMatrix(nrows, ncols)   // use a linear storage
    {
        cout << "Hello from DEN Constructor" << endl;
    }

    template <class fp_type>
    void SparseMatrix_DEN<fp_type>::assemble()
    {
        cout << "Hello from DEN assemble" << endl;

        //This routine needs to convert _buildCoeff into the COO storage format.
    }
    template <class fp_type>
    std::vector<fp_type> SparseMatrix_DEN<fp_type>::matvec(const std::vector<fp_type>& x) const
    {
        // this is a temperary version of matvec
        assert(this->_state == MatrixState::assembled);
        assert(x.size() == this->_ncols);

        std::vector<fp_type> y(this->_nrows, static_cast<fp_type>(0));

        // Perform dense matrix-vector multiplication
        for (size_t i = 0; i < this->_nrows; ++i)
        {
            for (size_t j = 0; j < this->_ncols; ++j)
            {
                y[i] += _data[i * this->_ncols + j] * x[j];
            }
        }

        return y;
    }

}

// Need to declare the concrete templates within the library for
// use in code that links to libspmv
template class SpMV::SparseMatrix_DEN<float>;
template class SpMV::SparseMatrix_DEN<double>;