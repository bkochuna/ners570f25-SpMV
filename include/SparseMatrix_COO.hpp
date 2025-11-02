#ifndef __SPMV_SparseMatrix_COO__
#define __SPMV_SparseMatrix_COO__

#include "SparseMatrix.hpp"

namespace SpMV
{
    template <class fp_type>
    class SparseMatrix_COO : public SparseMatrix<fp_type>
    {
        public:
            SparseMatrix_COO(const size_t nrows, const size_t ncols);
            void assemble();


		// Create a friend class to access private variables
		#ifdef COO_UNIT_TEST
		template <typename T>
		friend class TestSparseMatrixCOO;
		#endif

    };
}

#endif
