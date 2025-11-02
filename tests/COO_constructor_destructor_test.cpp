// tests/COO_constructor_destructor_test.cpp
#include <SpMV.hpp>
#include <iostream>
#include <stdexcept>

#include "unit_test_framework.hpp"

// get methods where added as public to the SparseMatrix base class
template <typename T,size_t N, size_t M>
TEST_CASE(test_COO_constructor)
{
    try {
        SpMV::SparseMatrix_COO<T> A(N,M); 
        ASSERT(A.nrows() == N);
        ASSERT(A.ncols() == M);
        ASSERT(A.numnz() == 0);
        ASSERT(A.getState() == SpMV::MatrixState::initialized);
        ASSERT(A.buildCoeffEmpty());
        std::cout << "COO constructor test pass (" << N << " by " << M << ")\n";
    }
    catch (const std::exception &e){
        std::cerr << "COO constructor test fail" << e.what() << std::endl;
        ASSERT(false);
    }
}

template <typename T, size_t N, size_t M>
TEST_CASE(test_COO_destructor_assembled)
{ 
    try {
            {// begin scope exiting scope automatically calls destructor  
                SpMV::SparseMatrix_COO<T> A(N,M); 
                A.setValue(N-1,M-1, static_cast<T>(2.71));
                A.setValue(0,0, static_cast<T>(42.0));
                A.assemble();
        
            } // destructor is called
        std::cout << "COO assembled destructor test pass (" << N << " by " << M << ")\n";
        } 
    catch (const std::exception &e){
        std::cerr << "COO assembled destructor test fail" << e.what() << std::endl;
        ASSERT(false);
    }

}

template <typename T, size_t N, size_t M>
TEST_CASE(test_COO_destructor)
{ 
    try {
            {// begin scope exiting scope automatically calls destructor  
                SpMV::SparseMatrix_COO<T> A(N,M); 
        
            } // destructor is called
        std::cout << "COO destructor test pass (" << N << " by " << M << ")\n";
        } 
    catch (const std::exception &e){
        std::cerr << "COO destructor test fail" << e.what() << std::endl;
        ASSERT(false);
    }

}


TEST_SUITE(my_suite) {
    TEST((test_COO_constructor<double, 3, 2>));
    TEST((test_COO_destructor<double, 2, 3>));
    TEST((test_COO_destructor_assembled<double, 2, 3>));
    TEST((test_COO_constructor<float, 4, 5>));
    TEST((test_COO_destructor<float, 5, 4>));
    TEST((test_COO_destructor_assembled<float, 5, 4>));

} 

auto
main() -> int
{
  // Run the unit tests. If a test fails, the program will print failure info
  // and return 1.
  RUN_SUITE(my_suite);
  return 0; 
}