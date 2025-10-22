#include "SparseMatrix_DEN.hpp"

#include <iostream>
#include <algorithm>
#include <cassert>


namespace SpMV
{
    //constractor copy from Ethan to align interface 
    template <class fp_type>
    SparseMatrix_DEN<fp_type>::SparseMatrix_DEN(const size_t nrows, const size_t ncols)
        : SparseMatrix<fp_type>::SparseMatrix(nrows, ncols)
    {
        std::cout << "Hello from SparseMatrix_DEN Constructor!" << std::endl;

        // Allocate 2D dense matrix
        _Matrix = std::make_unique<std::unique_ptr<fp_type[]>[]>(this->_nrows);
        for (size_t i = 0; i < this->_nrows; ++i)
        {
            _Matrix[i] = std::make_unique<fp_type[]>(this->_ncols);
            std::fill(_Matrix[i].get(), _Matrix[i].get() + this->_ncols, static_cast<fp_type>(0));
        }

        // Mark state as initialized
        this->_state = MatrixState::initialized;
    }


    template <class fp_type>
    SparseMatrix_DEN<fp_type>::~SparseMatrix_DEN() {}

    template <class fp_type>
    void SparseMatrix_DEN<fp_type>::assemble()
    {
        cout << "Hello from DEN assemble" << endl;
    }


    // matvac from kunjian
    template <class fp_type>
    std::vector<fp_type> SparseMatrix_DEN<fp_type>::matvec(const std::vector<fp_type>& x) const{
        assert(this->_state == MatrixState::assembled);
        assert(x.size() == this->_ncols);

        std::vector<fp_type> y(this->_nrows, static_cast<fp_type>(0));

        for (size_t i = 0; i < this->_nrows; ++i){
            for (size_t j = 0; j < this->_ncols; ++j){
                y[i] += _Matrix[i][j] * x[j];
            }
        }

        return y;
    }

}

// Need to declare the concrete templates within the library for
// use in code that links to libspmv
template class SpMV::SparseMatrix_DEN<float>;
template class SpMV::SparseMatrix_DEN<double>;