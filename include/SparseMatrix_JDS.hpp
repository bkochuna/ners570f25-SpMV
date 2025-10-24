#ifndef __SPMV_SparseMatrix_JDS__
#define __SPMV_SparseMatrix_JDS__

#include "SparseMatrix.hpp"
#include <vector>

namespace SpMV
{
    template <class fp_type>
    class SparseMatrix_JDS : public SparseMatrix<fp_type>
    {
    public:
        // JDS data members
        std::vector<fp_type> val;
        std::vector<size_t> col_idx;
        std::vector<size_t> perm;
        std::vector<size_t> jd_ptr;

        // Constructor and destructor
        SparseMatrix_JDS(const size_t nrows, const size_t ncols);
        ~SparseMatrix_JDS() override = default;

        // Empty assemble
        void assemble() override;
        
	// Implement matveck
        void matvec(const std::vector<fp_type>& x, std::vector<fp_type>& y) const;


	// Accessor Methods
	// Array accessors (Map these to Lecture 12 naming in src)
	const fp_type* get_jdiag() const;
	const size_t* get_col_ind() const;
	const size_t* get_perm() const;
	const size_t* get_jd_ptr() const;

	// Scalar accessors
	size_t get_nz() const;
        size_t get_ncol() const;
        size_t get_nrow() const;
	size_t get_maxnz_row() const;
    };
}

#endif


