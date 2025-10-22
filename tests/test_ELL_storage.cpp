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

	//check zero padding
	for (size_t idx = 0; idx < mat.colIdx().size(); ++idx) {
		ASSERT(mat.colIdx()[idx] != std::numeric_limits<size_t>::max() || mat.values()[idx] == 0);
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

        //check zero padding
        for (size_t idx = 0; idx < mat.colIdx().size(); ++idx) {
                ASSERT(mat.colIdx()[idx] != std::numeric_limits<size_t>::max() || mat.values()[idx] == 0);
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

        //check zero padding
        for (size_t idx = 0; idx < mat.colIdx().size(); ++idx) {
                ASSERT(mat.colIdx()[idx] != std::numeric_limits<size_t>::max() || mat.values()[idx] == 0);
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




