#include "SpMV.hpp"                // must be before UT framework
#include "unit_test_framework.hpp" // per framework rules

#include <stdexcept>
#include <iostream>

// ==========================================================
// Unit Tests
// ==========================================================

TEST_CASE(test_validAccessorReads) {
    SpMV::SparseMatrix_ELL<float> A(3, 3, 2);

    A.set(0, 0, 0, 1.1f);
    A.set(0, 1, 1, 2.2f);
    A.set(1, 0, 2, 3.3f);

    ASSERT_NEAR(A.getValue(0,0), 1.1f, 1e-6f);
    ASSERT(A.getColIndex(1,0) == 2);
}

TEST_CASE(test_outOfRangeAccessThrows) {
    SpMV::SparseMatrix_ELL<float> A(2, 2, 2);

    bool caught = false;
    try {
        A.getValue(3, 0); // invalid row
    } catch (const std::out_of_range&) {
        caught = true;
    }
    ASSERT(caught);
}

TEST_CASE(test_paddedZeroAccess) {
    SpMV::SparseMatrix_ELL<float> A(2, 2, 3); // has padding

    A.set(0, 0, 0, 5.0f);

    ASSERT_NEAR(A.getValue(0,1), 0.0f, 1e-6f);
    ASSERT(A.getColIndex(0,1) == 0);
}

TEST_CASE(test_immutableAccessors) {
    SpMV::SparseMatrix_ELL<float> A(1, 1, 1);

    A.set(0, 0, 0, 9.9f);

    const auto& cref = A;

    ASSERT_NEAR(cref.getValue(0,0), 9.9f, 1e-6f);
    ASSERT(cref.getColIndex(0,0) == 0);
}

TEST_CASE(test_edgeCases) {
    SpMV::SparseMatrix_ELL<float> empty(0,0,0);
    ASSERT(empty.num_rows() == 0);
    ASSERT(empty.num_cols() == 0);

    SpMV::SparseMatrix_ELL<float> s(1,1,1);
    s.set(0,0,0,42.0f);
    ASSERT_NEAR(s.getValue(0,0),42.0f,1e-6f);

    SpMV::SparseMatrix_ELL<float> dense(2,2,2);
    dense.set(0,0,0,1.0f); dense.set(0,1,1,2.0f);
    dense.set(1,0,0,3.0f); dense.set(1,1,1,4.0f);
    ASSERT_NEAR(dense.getValue(1,1),4.0f,1e-6f);
}

TEST_CASE(test_invalidInputHandling) {
    SpMV::SparseMatrix_ELL<float> A(2, 2, 2);

    bool caught = false;
    try {
        size_t bad = static_cast<size_t>(-1);
        A.getValue(bad, 0);
    } catch (const std::out_of_range&) {
        caught = true;
    }
    ASSERT(caught);
}


// ==========================================================
// Suite + main
// ==========================================================

TEST_SUITE(ELL_Accessors_Suite) {
    TEST(test_validAccessorReads);
    TEST(test_outOfRangeAccessThrows);
    TEST(test_paddedZeroAccess);
    TEST(test_immutableAccessors);
    TEST(test_edgeCases);
    TEST(test_invalidInputHandling);
}

int main() {
    RUN_SUITE(ELL_Accessors_Suite);
    return 0;
}
