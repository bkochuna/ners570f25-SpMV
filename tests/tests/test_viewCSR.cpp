#include <SpMV.hpp>          // Must include CSR class (once implemented)
#include "unit_test_framework.hpp"

#include <sstream>
#include <iostream>
#include <vector>

using namespace SpMV;

// Helper to capture output of A.view()
template <typename MatrixType>
std::string capture_view(const MatrixType& A) {
    std::ostringstream oss;
    std::streambuf* old = std::cout.rdbuf(oss.rdbuf());
    A.view();                // or A.viewCSR()
    std::cout.rdbuf(old);
    return oss.str();
}

TEST_CASE(csr_view_basic)
{
    // Matrix:
    // [10 0 0]
    // [0 20 0]
    // [0 0 30]
    SparseMatrix_CSR<double> A(3, 3);
    A.setValue(0, 0, 10.0);
    A.setValue(1, 1, 20.0);
    A.setValue(2, 2, 30.0);
    A.assemble();

    std::string output = capture_view(A);

    std::string expected =
        "Row 0: (0, 10)\n"
        "Row 1: (1, 20)\n"
        "Row 2: (2, 30)\n";

    ASSERT(output == expected);
}

TEST_SUITE(csr_suite)
{
    TEST(csr_view_basic);
}

auto main() -> int
{
    RUN_SUITE(csr_suite);
    return 0;
}
