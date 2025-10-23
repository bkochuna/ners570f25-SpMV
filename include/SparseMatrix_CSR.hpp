#ifndef __SPMV_SparseMatrix_CSR__
#define __SPMV_SparseMatrix_CSR__

#include "SparseMatrix.hpp"   // Task: base class available for inheritance
#include <vector>             // Task: CSR attributes (rowPtr, colInd, values) use std::vector
#include <ostream>            // Task: function stubs (view) needs std::ostream

namespace SpMV
{
    // Definition of done: class correctly inherits from SparseMatrix
    template <class fp_type>
    class SparseMatrix_CSR : public SparseMatrix<fp_type>
    {
        public:
            // Definition of done: default constructor must exist
            // Task: constructor uses an initializer list (implemented in .cpp)
            SparseMatrix_CSR();

            // Definition of done: constructor with one argument per member to directly assemble the class
            // Task: constructor uses an initializer list (implemented in .cpp)
            SparseMatrix_CSR(const size_t nrows, const size_t ncols,
                             std::vector<size_t> rowPtr,
                             std::vector<size_t> colInd,
                             std::vector<fp_type> values);

            // Definition of done: destructor frees memory and nulls pointers
            virtual ~SparseMatrix_CSR();

            // Task: add comments/function stubs for future methods (declared only; implemented later)
            void view(std::ostream& os) const;
            void matvec(const fp_type* x, fp_type* y) const;
            void fromCOO(const size_t nrows, const size_t ncols,
                         const std::vector<size_t>& I,
                         const std::vector<size_t>& J,
                         const std::vector<fp_type>& V);

        private:
            // Task: decide on CSR attributes (m,n are in base; CSR owns these)
            std::vector<size_t>  _rowPtr;   // length = m+1
            std::vector<size_t>  _colInd;   // length = nnz
            std::vector<fp_type> _values;   // length = nnz
            // Task: choose access specifiers — public API above, private data here
    };
}

#endif
