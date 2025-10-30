#ifndef __SPMV_SPARSEMATRIX_DEN__
#define __SPMV_SPARSEMATRIX_DEN__

#include "SparseMatrix.hpp"
#include <memory>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <cassert>

namespace SpMV
{
    template <class fp_type>
    class SparseMatrix_DEN : public SparseMatrix<fp_type>
    {
        private:
            std::unique_ptr<std::unique_ptr<fp_type[]>[]> _Matrix; // Declare Matrix


        public:
            SparseMatrix_DEN(const size_t nrows, const size_t ncols); // Constructor
            ~SparseMatrix_DEN(); // Destructor
            
            void assemble() override;
            void assembleStorage(); // overloaded to work with test expectations
            void assembleStorage(
                    const std::vector<size_t>& I,
                    const std::vector<size_t>& J,
                    const std::vector<fp_type>&  V,
                    size_t m, size_t n);
            void disassembleStorage();

            fp_type getValue(const size_t i, const size_t j);
            std::vector<fp_type> matvec(const std::vector<fp_type>& x) const;
            // Getters for testing
            size_t nrows() const { return this->_nrows; }
            size_t ncols() const { return this->_ncols; }
            size_t numnz() const { return this->_numnz; }
            MatrixState getState() const { return this->_state; }
            bool buildCoeffEmpty() const { return this->_buildCoeff.empty(); }
            size_t buildCoeffSize() const { return this->_buildCoeff.size(); }
            std::vector<fp_type> values() const;
    };

}

#endif