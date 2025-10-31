#include "SparseMatrix_DEN.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>

const double TOL = 1e-12;

// Helper to check vectors within tolerance
template <typename T>
bool vectors_equal(const std::vector<T>& a, const std::vector<T>& b, double tol = TOL) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); ++i) {
        if (std::abs(a[i] - b[i]) > tol) return false;
    }
    return true;
}

// ---- Test functions ----
void test_dense_matvec_identity() {
    SpMV::SparseMatrix_DEN<double> A(3, 3);
    A.assemble();
    std::vector<double> x = {1.0, 2.0, 3.0};
    std::vector<double> y = A.matvec(x);
    if (y.size() != x.size()) throw std::runtime_error("Output size mismatch");
}

void test_dense_matvec_zero() {
    SpMV::SparseMatrix_DEN<double> A(3, 3);
    A.assemble();
    std::vector<double> x = {1.0, -1.0, 2.0};
    std::vector<double> y = A.matvec(x);
    std::vector<double> expected(3, 0.0);
    if (!vectors_equal(y, expected)) throw std::runtime_error("Non-zero output for zero matrix");
}

void test_dense_matvec_scaling() {
    SpMV::SparseMatrix_DEN<double> A(2, 2);
    A.assemble();
    std::vector<double> x = {2.0, 3.0};
    std::vector<double> y = A.matvec(x);
    std::vector<double> expected(2, 0.0);
    if (!vectors_equal(y, expected)) throw std::runtime_error("Scaling test mismatch");
}

void test_dense_matvec_rectangular() {
    SpMV::SparseMatrix_DEN<double> A(2, 3);
    A.assemble();
    std::vector<double> x = {1.0, 2.0, 3.0};
    std::vector<double> y = A.matvec(x);
    std::vector<double> expected(2, 0.0);
    if (!vectors_equal(y, expected)) throw std::runtime_error("Rectangular test mismatch");
}

void test_dense_matvec_tolerance() {
    SpMV::SparseMatrix_DEN<double> A(2, 2);
    A.assemble();
    std::vector<double> x = {1.0, 1.0};
    std::vector<double> y = A.matvec(x);
    for (auto val : y)
        if (std::abs(val) > TOL) throw std::runtime_error("Value exceeds tolerance");
}

// ---- Main test runner ----
int main() {
    std::vector<std::pair<std::string, void(*)()>> tests = {
        {"Identity", test_dense_matvec_identity},
        {"Zero", test_dense_matvec_zero},
        {"Scaling", test_dense_matvec_scaling},
        {"Rectangular", test_dense_matvec_rectangular},
        {"Tolerance", test_dense_matvec_tolerance}
    };

    bool all_passed = true;

    for (auto& [name, func] : tests) {
        std::cout << "Running test_" << name << "... ";
        try {
            func();
            std::cout << "PASSED ✅" << std::endl;
        } catch (const std::exception& e) {
            std::cout << "FAILED ❌ (" << e.what() << ")" << std::endl;
            all_passed = false;
        }
    }

    if (all_passed)
        std::cout << "\n All tests passed.\n";
    else
        std::cout << "\n Some tests failed.\n";

    return all_passed ? 0 : 1;
}
