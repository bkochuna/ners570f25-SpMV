#include <SpMV.hpp>
#include "SparseMatrix_ELL.hpp"
#include "unit_test_framework.hpp"
#include <vector>
#include <iostream>
#include <limits>

using namespace SpMV;

TEST_CASE(test_ELL_storage_4by4) 
{
	//4x4 case
	size_t nrows = 4;
	size_t ncols = 4;

	std::vector<size_t> i_idx = {0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3};
	std::vector<size_t> j_idx = {0, 1, 0, 1, 2, 1, 2, 3, 0, 2, 3};
	std::vector<double> vals = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

	SparseMatrix_ELL<double> mat(nrows, ncols);

	mat.assembleStorage(i_idx, j_idx, vals, nrows, ncols);

	ASSERT(!mat.colIdx().empty());
	ASSERT(!mat.values().empty());

	//expected size?
	size_t expected_size = mat.nrows() * mat.maxEntriesPerRow();
	ASSERT(mat.colIdx().size() == expected_size);
	ASSERT(mat.values().size() == expected_size);

	//check equality
        std::vector<size_t> expected_colIdx = {0, 0, 1, 0, 1, 1, 2, 2, std::numeric_limits<size_t>::max(), 2, 3, 3 };
	std::vector<double> expected_vals = {1, 3, 6, 9, 2, 4, 7, 10, 0, 5, 8, 11};

        for (size_t idx = 0; idx < expected_size; ++idx) {
                ASSERT(mat.colIdx()[idx] == expected_colIdx[idx]);
                ASSERT(mat.values()[idx] == expected_vals[idx]);
        }

        // check zero padding
        for (size_t idx = 0; idx < expected_size; ++idx) {
                if (mat.colIdx()[idx] == std::numeric_limits<size_t>::max()) {
                        ASSERT(mat.values()[idx] == 0.0);
                }
        }

        //now check disassembleStorage()
        auto out = mat.disassembleStorage();

        ASSERT(out.i.size() == out.j.size());
        ASSERT(out.i.size() == out.val.size());
        ASSERT(out.i.size() == i_idx.size());

        for (size_t k = 0; k < i_idx.size(); ++k) {
                ASSERT(out.i[k] == i_idx[k]);
                ASSERT(out.j[k] == j_idx[k]);
                ASSERT(out.val[k] == vals[k]);
        }
}


TEST_CASE(test_ELL_storage_8by8)
{
        //8x8 case
        size_t nrows = 8;
        size_t ncols = 8;

        std::vector<size_t> i_idx = {0, 0, 1, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7};
        std::vector<size_t> j_idx = {0, 2, 1, 3, 4, 2, 5, 0, 3, 1, 6, 2, 7, 0, 6, 1, 7};
        std::vector<double> vals = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};

        SparseMatrix_ELL<double> mat(nrows, ncols);

        mat.assembleStorage(i_idx, j_idx, vals, nrows, ncols);

        ASSERT(!mat.colIdx().empty());
        ASSERT(!mat.values().empty());

        //expected size?
        size_t expected_size = mat.nrows() * mat.maxEntriesPerRow();

        ASSERT(mat.colIdx().size() == expected_size);
        ASSERT(mat.values().size() == expected_size);

        //check equality
        std::vector<size_t> expected_colIdx = {0, 1, 2, 0, 1, 2, 0, 1, 2, 3, 5, 3, 6, 7, 6, 7, std::numeric_limits<size_t>::max(), 4, std::numeric_limits<size_t>::max(), std::numeric_limits<size_t>::max(), std::numeric_limits<size_t>::max(), std::numeric_limits<size_t>::max(), std::numeric_limits<size_t>::max(), std::numeric_limits<size_t>::max()};
        std::vector<double> expected_vals = {1, 3, 6, 8, 10, 12, 14, 16, 2, 4, 7, 9, 11, 13, 15, 17, 0, 5, 0, 0, 0, 0, 0, 0};

        for (size_t idx = 0; idx < expected_size; ++idx) {
                ASSERT(mat.colIdx()[idx] == expected_colIdx[idx]);
                ASSERT(mat.values()[idx] == expected_vals[idx]);
        }

        // check zero padding
        for (size_t idx = 0; idx < expected_size; ++idx) {
                if (mat.colIdx()[idx] == std::numeric_limits<size_t>::max()) {
                        ASSERT(mat.values()[idx] == 0.0);
                }
        }

        //now check disassembleStorage()
        auto out = mat.disassembleStorage();

        ASSERT(out.i.size() == out.j.size());
        ASSERT(out.i.size() == out.val.size());
        ASSERT(out.i.size() == i_idx.size());

        for (size_t k = 0; k < i_idx.size(); ++k) {
                ASSERT(out.i[k] == i_idx[k]);
                ASSERT(out.j[k] == j_idx[k]);
                ASSERT(out.val[k] == vals[k]);
        }
}

TEST_CASE(test_ELL_storage_16by16)
{
        //16x16 case
        size_t nrows = 16;
        size_t ncols = 16;

        std::vector<size_t> i_idx = {0, 0, 1, 1, 2, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 12, 12, 13, 13, 14, 14, 15};
        std::vector<size_t> j_idx = {0, 3, 1, 4, 5, 0, 1, 6, 2, 7, 0, 6, 1, 7, 2, 8, 3, 9, 4, 10, 5, 6, 12, 7, 13, 8, 14, 15};
        std::vector<double> vals = {1, 2, 3, 4, 6, 7, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 25, 26, 27, 28, 29, 30, 32};

        SparseMatrix_ELL<double> mat(nrows, ncols);

        mat.assembleStorage(i_idx, j_idx, vals, nrows, ncols);

        ASSERT(!mat.colIdx().empty());
        ASSERT(!mat.values().empty());

        //expected size?
        size_t expected_size = mat.nrows() * mat.maxEntriesPerRow();

        ASSERT(mat.colIdx().size() == expected_size);
        ASSERT(mat.values().size() == expected_size);

        //check equality
        std::vector<size_t> expected_colIdx = {0, 1, 5, 0, 1, 2, 0, 1, 2, 3, 4, 5, 6, 7, 8, 15, 3, 4, std::numeric_limits<size_t>::max(), std::numeric_limits<size_t>::max(), 6, 7, 6, 7, 8, 10, 10, std::numeric_limits<size_t>::max()};
        std::vector<double> expected_vals = {1, 3, 6, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 32, 2, 4, 0, 0, 10, 12, 14, 16, 18, 20, 22, 0};

        for (size_t idx = 0; idx < expected_size; ++idx) {
                ASSERT(mat.colIdx()[idx] == expected_colIdx[idx]);
                ASSERT(mat.values()[idx] == expected_vals[idx]);
        }

        // check zero padding
        for (size_t idx = 0; idx < expected_size; ++idx) {
                if (mat.colIdx()[idx] == std::numeric_limits<size_t>::max()) {
                        ASSERT(mat.values()[idx] == 0.0);
                }
        }

        //now check disassembleStorage()
        auto out = mat.disassembleStorage();

        ASSERT(out.i.size() == out.j.size());
        ASSERT(out.i.size() == out.val.size());
        ASSERT(out.i.size() == i_idx.size());

        for (size_t k = 0; k < i_idx.size(); ++k) {
                ASSERT(out.i[k] == i_idx[k]);
                ASSERT(out.j[k] == j_idx[k]);
                ASSERT(out.val[k] == vals[k]);
        }
}

TEST_SUITE(ELL_storage_suite) {
	TEST(test_ELL_storage_4by4);
	TEST(test_ELL_storage_8by8);
	TEST(test_ELL_storage_16by16);
}

auto main() -> int {
	RUN_SUITE(ELL_storage_suite);
	return 0;
}




