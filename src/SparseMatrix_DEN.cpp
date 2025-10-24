#include "SparseMatrix_DEN.hpp"
#include <iostream>
#include <cassert>

namespace SpMV
{
    template <class fp_type>
    SparseMatrix_DEN<fp_type>::SparseMatrix_DEN(const size_t nrows, const size_t ncols) :
        SparseMatrix<fp_type>::SparseMatrix(nrows, ncols)
    {
        std::cout << "Hello from DEN Constructor" << std::endl;
        _data.resize(nrows * ncols, static_cast<fp_type>(0)); // initialize zeros
    }

    template <class fp_type>
    SparseMatrix_DEN<fp_type>::~SparseMatrix_DEN()
    {
        std::cout << "DEN Destructor called" << std::endl;
    }

    template <class fp_type>
    void SparseMatrix_DEN<fp_type>::assemble()
    {
        std::cout << "Hello from DEN assemble" << std::endl;
        this->_state = MatrixState::assembled;
        // TODO: assembly logic
    }

    template <class fp_type>
    std::vector<fp_type> SparseMatrix_DEN<fp_type>::matvec(const std::vector<fp_type>& x) const
    {
        assert(this->_state == MatrixState::assembled);
        assert(x.size() == this->_ncols);

        std::vector<fp_type> y(this->_nrows, static_cast<fp_type>(0));

        for (size_t i = 0; i < this->_nrows; ++i)
        {
            for (size_t j = 0; j < this->_ncols; ++j)
            {
                y[i] += _data[i * this->_ncols + j] * x[j];
            }
        }

        return y;
    }

} // namespace SpMV

// ---------------------------
// Explicit template instantiations
// ---------------------------
template class SpMV::SparseMatrix_DEN<float>;
template class SpMV::SparseMatrix_DEN<double>;
