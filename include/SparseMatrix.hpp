#ifndef __SPMV_SparseMatrix__
#define __SPMV_SparseMatrix__

#include <stddef.h>
#include <map>
#include <utility>

using namespace std;

namespace SpMV
{

    // Enumerations for matrix state to support builder pattern
    enum MatrixState {undefined, initialized, building, assembled};

    // Enumerations for Matrix Format
    enum MatrixFormat {UDF, DEN, COO, CSR, ELL, JDS};

    template <class fp_type>
    class SparseMatrix
    {
    private:
        MatrixState _state = MatrixState::undefined;
             size_t _nrows = 0;
             size_t _ncols = 0;
             size_t _numnz = 0;

        const MatrixFormat _fmt;
             

    protected:
        std::map<std::pair<size_t, size_t>, fp_type> _buildCoeff;
        
        virtual void _disassemble() =0;
        virtual fp_type _getValueConcrete(const size_t i, const size_t j) const =0;
    public:
        SparseMatrix(const size_t nrows, const size_t ncols);
        virtual ~SparseMatrix();

        inline void         setState(MatrixState S) { this->_state = S; };
        inline MatrixState  getState()          const noexcept { return _state; };
        inline size_t       getNumRows()        const noexcept { return _nrows; };
        inline size_t       getNumCols()        const noexcept { return _ncols; };
        inline size_t       getNumNonZeros()    const noexcept { return _numnz; };
        virtual inline MatrixFormat getFormat() const noexcept { return _fmt; };

        void    setValue(const size_t i, const size_t j, fp_type val);
        fp_type getValue(const size_t i, const size_t j) const;
        virtual void assemble() =0; // The "=0" defines this class as abstract
        virtual void matvec() =0;
    };
}

#endif