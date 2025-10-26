#pragma once

#include "unit_test_framework.hpp"
#include "SparseMatrix_CSR.hpp"
#include <vector>
#include <stdexcept>

namespace SpMV
{
    template <typename T>
    SparseMatrix_CSR<T> createTestCSR();

    template <typename T>
    TEST_CASE(testCSRGetters);

    template <typename T>
    TEST_CASE(testCSRUninitializedThrows);

    template <typename T>
    TEST_SUITE(CSRGettersSuite);
} // namespace SpMV


auto main() -> int;
