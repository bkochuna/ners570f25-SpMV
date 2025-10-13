#ifndef __SPMV_SparseMatrix__
#define __SPMV_SparseMatrix__

#include <stddef.h>
#include <map>
#include <utility>

using namespace std;

namespace SpMV
{

    enum MatrixState {undefined, initialized, building, assembled};

    template <class fp_type>
    class SparseMatrix
    {
    protected:
        MatrixState _state = MatrixState::undefined;
             size_t _nrows = 0;
             size_t _ncols = 0;
             size_t _numnz = 0;

        std::map<std::pair<size_t, size_t>, fp_type> _buildCoeff;
        

    public:
        SparseMatrix(const size_t nrows, const size_t ncols);
        virtual ~SparseMatrix();

        void    setValue(const size_t i, const size_t j, fp_type val);
        fp_type getValue(const size_t i, const size_t j);
        virtual void assemble() =0; // The "=0" defines this class as abstract
    };
}

#endif