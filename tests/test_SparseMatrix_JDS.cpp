#include <SpMV.hpp>                
#include "unit_test_framework.hpp" 

#include <iostream>
#include <type_traits>  
#include <vector>
#include <stdexcept>

using namespace SpMV;


TEST_CASE(test_JDS_constructor_initialization)
{
    SparseMatrix_JDS<double> A(4, 4);

    ASSERT(A._nrows == 4);
    ASSERT(A._ncols == 4);

    ASSERT(A._state == MatrixState::initialized);
}


TEST_SUITE(JDS_suite)
{
    TEST(test_JDS_constructor_initialization);
}

int main()
{
    RUN_SUITE(JDS_suite);
    return 0;
}
