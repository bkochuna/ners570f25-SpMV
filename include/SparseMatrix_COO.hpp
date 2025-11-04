#ifndef __SPMV_SparseMatrix_COO__
#define __SPMV_SparseMatrix_COO__

#include "SparseMatrix.hpp"

namespace SpMV
{
    template <class fp_type>
    class SparseMatrix_COO : public SparseMatrix<fp_type>
    {
        private:
            // Abstract private methods from base class
            void _disassemble() override;
            fp_type _getValueConcrete(const size_t i, const size_t j) const override;

            // Concrete storage for COO
            size_t*    _I = nullptr;
            size_t*    _J = nullptr;
            fp_type* _val = nullptr;
        
            const MatrixFormat _fmt;
        public:
            SparseMatrix_COO(const size_t nrows, const size_t ncols);
            ~SparseMatrix_COO() override;
            void assemble() override;
            void matvec() override;
    };
}

#endif