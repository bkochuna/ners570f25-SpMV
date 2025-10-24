#pragma once
#include <vector>
#include <numeric>
#include <algorithm>
#include <cassert>

namespace spmv {

using index_t = int;
using value_t = double;

struct COO {
    int nrows=0, ncols=0;
    std::vector<index_t> row, col;
    std::vector<value_t> val;
};

struct CSR {
    int nrows=0, ncols=0;
    std::vector<index_t> rowptr;
    std::vector<index_t> col;
    std::vector<value_t> val;
};

struct JDS {
    int nrows=0, ncols=0;
    std::vector<index_t> perm;
    std::vector<index_t> jds_ptr;
    std::vector<index_t> col_idx;
    std::vector<value_t> vals;
};

JDS assemble_fromCSR(const CSR& A);
JDS assemble_fromCOO(const COO& A);
COO disassemble_toCOO(const JDS& A);

} // namespace spmv
