#include "SparseMatrix_COO.hpp"

#include <iostream>
#include <stddef.h>
#include <cassert>

using namespace std;

namespace SpMV
{
    template <class fp_type>
    SparseMatrix_COO<fp_type>::SparseMatrix_COO(const size_t nrows, const size_t ncols) :
        SparseMatrix<fp_type>::SparseMatrix(nrows, ncols), _fmt(MatrixFormat::COO)
    {
        //cout << "Hello from COO Constructor" << endl;
    }

    template <class fp_type>
    SparseMatrix_COO<fp_type>::~SparseMatrix_COO()
    {
        if(this->_I != nullptr) {
            delete[] this->_I;
            this->_I = nullptr;
        }

        if(this->_J != nullptr) {
            delete[] this->_J;
            this->_J = nullptr;
        }

        if(this->_val != nullptr) {
            delete[] this->_val;
            this->_val = nullptr;
        }

        assert(this->_I == nullptr);
        assert(this->_J == nullptr);
        assert(this->_val == nullptr);
    }

    template <class fp_type>
    void SparseMatrix_COO<fp_type>::_disassemble()
    {
        assert(this->getState() == MatrixState::assembled);
        this->setState(MatrixState::building);

        size_t nnz = this->getNumNonZeros();
        for(size_t n = 0; n < nnz; ++n) {
            size_t    i = this->_I[n];
            size_t    j = this->_J[n];
            fp_type aij = this->_val[n];
            this->setValue(i,j,aij);
        }
        
        delete[] this->_I;   this->_I   = nullptr;
        delete[] this->_J;   this->_J   = nullptr;
        delete[] this->_val; this->_val = nullptr;

        assert(this->getState() == MatrixState::building);
        assert(this->_I   == nullptr);
        assert(this->_J   == nullptr);
        assert(this->_val == nullptr);
    }

    template <class fp_type>
    fp_type SparseMatrix_COO<fp_type>::_getValueConcrete(const size_t i, const size_t j) const
    {
        assert(i < this->getNumRows());
        assert(j < this->getNumCols());
        assert(this->getState() == MatrixState::assembled);

        for(size_t n = 0; n < this->getNumNonZeros(); ++n) {
            if(i == this->_I[n] && j == this->_J[n])
                return this->_val[n];
        }

        return 0.0;
    }

    template <class fp_type>
    void SparseMatrix_COO<fp_type>::assemble()
    {
        assert(this->getState() == MatrixState::building);

        if(this->getNumNonZeros() > 0)
        this->_I   = new  size_t[this->getNumNonZeros()];
        this->_J   = new  size_t[this->getNumNonZeros()];
        this->_val = new fp_type[this->getNumNonZeros()];

        size_t nnz = 0;
        for(const auto& [key, val] : this->_buildCoeff) {
            this->  _I[nnz] = key.first;
            this->  _J[nnz] = key.second;
            this->_val[nnz] = val;
            nnz++;
        }

        this->_buildCoeff.clear();        
        this->setState(MatrixState::assembled);

        assert(this->getState() == MatrixState::assembled);
        assert(this->_buildCoeff.empty());
    }

    template <class fp_type>
    void SparseMatrix_COO<fp_type>::matvec()
    {
        //Not implemented yet
    }
}

// Need to declare the concrete templates within the library for
// use in code that links to libspmv
template class SpMV::SparseMatrix_COO<float>;
template class SpMV::SparseMatrix_COO<double>;
