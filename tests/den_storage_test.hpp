// tests/den_storage_tests.hpp
#pragma once

#include <vector>
#include <cstddef>

// Helper declaration (defined in the .cpp):
// Build a row-major dense vector of size m*n from (I,J,V) triplets.
// Overwrite policy on duplicate entries (last wins).
template <typename T>
std::vector<T> make_dense_row_major(
    std::size_t m, std::size_t n,
    const std::vector<std::size_t>& I,
    const std::vector<std::size_t>& J,
    const std::vector<T>& V);

    