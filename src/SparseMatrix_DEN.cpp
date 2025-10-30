#include "SparseMatrix_DEN.hpp"
#include <iostream>
#include <memory>
#include <algorithm>
#include <string>
#include <cmath>
#include <cassert>
#include <stddef.h>
#include <vector>
#include <stdexcept>

namespace SpMV
{

	////// Constructor Definition
    template <class fp_type>
    SparseMatrix_DEN<fp_type>::SparseMatrix_DEN(const size_t nrows, const size_t ncols) :
        SparseMatrix<fp_type>::SparseMatrix(nrows, ncols)
    {

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
        assert(this->_state == MatrixState::initialized || this->_state==MatrixState::building);
        if (this->_state == MatrixState::initialized) {
            // nothing in _buildCoeff yet, just mark assembled
            this->_state = MatrixState::assembled;
        }
        else {
            std::vector<size_t> I(this->_buildCoeff.size());
            std::vector<size_t> J(this->_buildCoeff.size());
            std::vector<fp_type> V(this->_buildCoeff.size());
            size_t k = 0;
            for (const auto & [ij,v] : this->_buildCoeff) {
                I[k] = ij.first;
                J[k] = ij.second;
                V[k] = v;
                ++k;
            }
            assembleStorage(I, J, V, this->_nrows, this->_ncols);
        }

    }
	
    template <class fp_type>
    void SparseMatrix_DEN<fp_type>::assembleStorage(
                                                const std::vector<size_t>& I,
                                                const std::vector<size_t>& J,
                                                const std::vector<fp_type>&  V,
                                                size_t m, size_t n) {
        assert(this->_state==MatrixState::initialized || this->_state==MatrixState::building);

        if (m != this->_nrows || n != this->_ncols){
            throw std::invalid_argument("assembleStorage: m,n incompatible with predefined nrows, ncols");
        }

        if (I.size() != J.size() || I.size() != V.size()){
            throw std::invalid_argument("assembleStorage: I, J, V size mismatch");
        }

        for (size_t k = 0; k < V.size(); ++k){
            if (!std::isfinite((V[k]))){
                throw std::invalid_argument("value is NaN/Inf");
            }
        }

        this->_buildCoeff.clear();
        for (size_t k = 0; k < V.size(); k++){
                size_t i = I[k];
                size_t j = J[k];
                if (i >= m || j >= n){
                    throw std::out_of_range("assembleStorage: index out of bounds");
                }
                this->_buildCoeff[{i,j}] = V[k];
            } 
        for (const auto& [ij, val] : this->_buildCoeff){
            _Matrix[ij.first][ij.second] = val;
        }
        
        this->_numnz = static_cast<size_t>(this->_buildCoeff.size());
        this->_state = MatrixState::assembled;

        this->_buildCoeff.clear(); 
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

    template <class fp_type>
    std::vector<fp_type> SparseMatrix_DEN<fp_type>::values() const {
        if (this->_state != MatrixState::assembled){
            throw std::logic_error("values() is not defined when storage is not assembled");
        }
        std::vector<fp_type> flat(this->_nrows * this->_ncols);
        for (size_t i =0; i < this->_nrows; ++i){
            for (size_t j = 0; j < this-> _ncols; ++j){
                flat[i*this->_ncols +j] = _Matrix[i][j];
            }
        }
        return flat;
    }
    
    ////// View Function(s)




	////// Matvec Function(s)
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
                y[i] += _Matrix[i][j] * x[j];
            }
        }
        return y;
    }

} // namespace SpMV

// Concrete template declaration
template class SpMV::SparseMatrix_DEN<float>;
template class SpMV::SparseMatrix_DEN<double>;
