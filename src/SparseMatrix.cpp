#include "SparseMatrix.hpp"

#include <iostream>
#include <cassert>

namespace SpMV
{

template <class fp_type>
SparseMatrix<fp_type>::SparseMatrix(const size_t nrows, const size_t ncols) :
    _nrows(nrows), _ncols(ncols)
{
    assert(this->_state == MatrixState::undefined);
    cout << "Hello from SparseMatrix Constructor!" << endl;
    this->_state = MatrixState::initialized;
    assert(this->_state == MatrixState::initialized);
}

template <class fp_type>
SparseMatrix<fp_type>::~SparseMatrix()
{
    cout << "Hello from SparseMatrix Destructor!" << endl;
    cout << "this->_nrows=" << this->_nrows << endl;
    cout << "this->_ncols=" << this->_ncols << endl;
    cout << "this->_numnz=" << this->_numnz << endl;
    this->_state=MatrixState::undefined;
}

template <class fp_type>
void SparseMatrix<fp_type>::setValue(const size_t i, const size_t j, fp_type val)
{
    assert(this->_state >= MatrixState::initialized);
    assert(i < this->_nrows);
    assert(j < this->_ncols);

    this->_state = MatrixState::building;

    //Store value
    this->_buildCoeff[std::make_pair(i,j)] = val;
    this->_numnz = this->_buildCoeff.size();

    cout << "i=" << i << endl;
    cout << "j=" << j << endl;
    cout << "val=" << val << endl;

    assert(this->_state == MatrixState::building);
}

template <class fp_type>
fp_type SparseMatrix<fp_type>::getValue(const size_t i, const size_t j)
{
    assert(this->_state > MatrixState::initialized);

    cout << "i=" << i << endl;
    cout << "j=" << j << endl;
    return 0.0;
}

}

// Need to declare the concrete templates within the library for
// use in code that links to libspmv
template class SpMV::SparseMatrix<float>;
template class SpMV::SparseMatrix<double>;
