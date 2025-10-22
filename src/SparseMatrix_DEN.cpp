#include "SparseMatrix_DEN.hpp"
#include <iostream>
#include <memory>
#include <algorithm>
#include <string>
#include <cmath>
#include <cassert>

namespace SpMV
{

	////// Constructor Definition
    template <class fp_type>
    SparseMatrix_DEN<fp_type>::SparseMatrix_DEN(const size_t nrows, const size_t ncols) :
        SparseMatrix<fp_type>::SparseMatrix(nrows, ncols)
    {
        std::cout << "Hello from SparseMatrix_DEN Constructor!" << std::endl;

        // Initialize Matrix
        _Matrix = std::make_unique<std::unique_ptr<fp_type[]>[]>(this->_nrows);

        for (size_t i = 0; i < this->_nrows; ++i) {
            _Matrix[i] = std::make_unique<fp_type[]>(this->_ncols);
            std::fill(_Matrix[i].get(), _Matrix[i].get() + this->_ncols, fp_type(0)); // changed to fp_type(0)
        }

        // Set State to Initialized (Parent Constructor Already Does This)
        this->_state = MatrixState::initialized;
    }


	////// Destructor Definition
    template <class fp_type>
    SparseMatrix_DEN<fp_type>::~SparseMatrix_DEN() {}



	////// Storage Function(s)
	template <class fp_type>
	void SparseMatrix_DEN<fp_type>::assemble() {
		assembleStorage();
	}

	template <class fp_type>
    void SparseMatrix_DEN<fp_type>::assembleStorage() {
        assert(this->_state == MatrixState::building);
        
        for (const auto & [ij,v] : this->_buildCoeff) {
            const size_t i = ij.first;
            const size_t j = ij.second;
            assert((i < this->_nrows) && (j < this->_ncols));
            _Matrix[i][j] = v;
        
        }

        this->_buildCoeff.clear();

        this->_state = MatrixState::assembled;
    }
	
    template <class fp_type>
    void SparseMatrix_DEN<fp_type>::disassembleStorage() {
        assert(this->_state == MatrixState::assembled);
        
        for (size_t i = 0; i < this->_nrows; ++i) {
            for (size_t j = 0; j < this->_ncols; ++j){
                fp_type v = _Matrix[i][j];
                if (std::fpclassify(v) != FP_ZERO){
                    this->_buildCoeff[{i,j}] = v;
                    _Matrix[i][j] = fp_type(0); // reset the matrix to the preallocated form of all zeros
                }
            }

        }

        this->_state = MatrixState::building;
    }


	////// Accessor Function(s)



	////// View Function(s)




	////// Matvec Function(s)
	

}

// Concrete template declaration
template class SpMV::SparseMatrix_DEN<float>;
template class SpMV::SparseMatrix_DEN<double>;