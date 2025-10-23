#include "SparseMatrix_DEN.hpp"
#include <iostream>
#include <memory>
#include <algorithm>
#include <string>
#include <cmath>


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
            std::fill(_Matrix[i].get(), _Matrix[i].get() + this->_ncols, 0.0);
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
		std::cout << "Placeholder for assemble Fcn" << std::endl;
	}


	////// Accessor Function
	template <class fp_type>
	fp_type SparseMatrix_DEN<fp_type>::getValue(const size_t i, const size_t j)
	{
		assert(this->_state >= MatrixState::initialized);
		
		cout << "i=" << i << endl;
		cout << "j=" << j << endl;

		if (this->_state == MatrixState::building) {
			auto found = this->_buildCoeff.find({i, j});
			if (found != this->_buildCoeff.end()){
				return found->second;}
			else {
				return _Matrix[i][j]; }
		}
		else {
			return _Matrix[i][j]; }
	}

    ////// View Function(s)




	////// Matvec Function(s)
	

}

// Concrete template declaration
template class SpMV::SparseMatrix_DEN<float>;
template class SpMV::SparseMatrix_DEN<double>;
