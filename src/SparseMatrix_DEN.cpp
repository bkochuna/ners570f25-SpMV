#include "SparseMatrix_DEN.hpp"
#include <memory>
#include <algorithm>
#include <string>
#include <cmath>


namespace SpMV
{

	////// Constructor Definition
    template <class fp_type>
    SparseMatrix_DEN<fp_type>::SparseMatrix_DEN(const size_t nrow, const size_t ncols) :
        SparseMatrix<fp_type>::SparseMatrix(nrows, ncols)
    {
        cout << "Hello from SparseMatrix_DEN Constructor!" << endl;

        // Initialize Matrix
        _Matrix = std::make_unique<std::unique_ptr<fp_type[]>[]>(this->_nrows);

        for (int i = 0; i < this->_nrows; ++i) {
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



	////// Accessor Function(s)



	////// View Function(s)




	////// Matvec Function(s)
	

}

// Concrete template declaration
template class SpMV::SparseMatrix_DEN<float>;
template class SpMV::SparseMatrix_DEN<double>;
