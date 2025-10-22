#ifndef __SPMV_SparseMatrix_DEN__
#define __SPMV_SparseMatrix_DEN__

#include "SparseMatrix.hpp"
#include <memory>

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
            void assemble();
	    
	    //Accesor Methods
	    fp_type getValue(const size_t i, const size_t j);
	    void setValue(const size_t i, const size_t j, fp_type val);
    };

}

#endif
