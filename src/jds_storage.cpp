#include "spmv/jds_storage.hpp"
#include <limits>

namespace spmv {

JDS assemble_fromCSR(const CSR& A) {
    JDS B;
    B.nrows = A.nrows;
    B.ncols = A.ncols;

    std::vector<int> rownnz(A.nrows);
    for (int i=0;i<A.nrows;i++)
        rownnz[i] = A.rowptr[i+1] - A.rowptr[i];

    B.perm.resize(A.nrows);
    std::iota(B.perm.begin(), B.perm.end(), 0);
    std::stable_sort(B.perm.begin(), B.perm.end(),
        [&](int a,int b){ return rownnz[a]>rownnz[b] || (rownnz[a]==rownnz[b] && a<b); });

    int maxlen = A.nrows ? *std::max_element(rownnz.begin(), rownnz.end()) : 0;
    int nnz = A.rowptr.back();

    B.jds_ptr.assign(maxlen+1, 0);
    std::vector<int> diagCount(maxlen, 0);
    for (int r_new=0; r_new<A.nrows; ++r_new) {
        int r = B.perm[r_new];
        for (int k=0; k<rownnz[r]; ++k)
            diagCount[k]++;
    }
    for (int d=0,acc=0; d<maxlen; ++d) { B.jds_ptr[d]=acc; acc+=diagCount[d]; }
    B.jds_ptr[maxlen]=nnz;

    B.col_idx.resize(nnz);
    B.vals.resize(nnz);
    std::vector<int> cursor = B.jds_ptr;

    for (int r_new=0; r_new<A.nrows; ++r_new) {
        int r = B.perm[r_new];
        int start=A.rowptr[r], end=A.rowptr[r+1];
        for (int k=0; k<end-start; ++k) {
            int d = k;
            int pos = cursor[d]++;
            B.col_idx[pos] = A.col[start+k];
            B.vals[pos]    = A.val[start+k];
        }
    }

    assert(static_cast<int>(B.jds_ptr.size()) == maxlen + 1);
    assert(B.jds_ptr.front()==0);
    assert(B.jds_ptr.back()==nnz);
    return B;
}

JDS assemble_fromCOO(const COO& in) {
    CSR A;
    A.nrows = in.nrows;
    A.ncols = in.ncols;
    A.rowptr.assign(A.nrows+1, 0);
    for (auto r : in.row) A.rowptr[r+1]++;
    for (int i=0;i<A.nrows;i++) A.rowptr[i+1]+=A.rowptr[i];
    assert(in.row.size() <= static_cast<size_t>(std::numeric_limits<int>::max()));
    int nnz = static_cast<int>(in.row.size());
    A.col.resize(nnz);
    A.val.resize(nnz);
    std::vector<int> offset = A.rowptr;
    for (size_t k=0;k<in.row.size();++k) {
        int r=in.row[k], pos=offset[r]++;
        A.col[pos]=in.col[k];
        A.val[pos]=in.val[k];
    }
    return assemble_fromCSR(A);
}

COO disassemble_toCOO(const JDS& B) {
    COO out;
    out.nrows = B.nrows;
    out.ncols = B.ncols;

    int maxlen = !B.jds_ptr.empty()
    ? static_cast<int>(B.jds_ptr.size()) - 1
    : 0;
    int nnz = maxlen ? B.jds_ptr.back() : 0;
    out.row.resize(nnz);
    out.col.resize(nnz);
    out.val.resize(nnz);

    for (int d=0; d<maxlen; ++d) {
        int s=B.jds_ptr[d], e=B.jds_ptr[d+1];
        for (int k=0; k<e-s; ++k) {
            int pos = s+k;
            int new_row = k;
            int old_row = B.perm[new_row];
            out.row[pos] = old_row;
            out.col[pos] = B.col_idx[pos];
            out.val[pos] = B.vals[pos];
        }
    }
    return out;
}

} // namespace spmv
