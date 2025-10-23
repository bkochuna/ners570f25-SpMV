#include <SpMV.hpp>                
#include "unit_test_framework.hpp" 

#include <iostream>
#include <type_traits>  
#include <memory>
#include <vector>
#include <stdexcept>

using namespace SpMV;

// initialization testing at different size
TEST_CASE(test_JDS_constructor_initialization_size8){
    SparseMatrix_JDS<double> A(8, 8);

    ASSERT(A._nrows == 8);
    ASSERT(A._ncols == 8);

    ASSERT(A._state == MatrixState::initialized);
}

TEST_CASE(test_JDS_constructor_initialization_size64){
    SparseMatrix_JDS<double> A(64, 64);

    ASSERT(A._nrows == 64);
    ASSERT(A._ncols == 64);

    ASSERT(A._state == MatrixState::initialized);
}

TEST_CASE(test_JDS_constructor_initialization_size256){
    SparseMatrix_JDS<double> A(256, 256);

    ASSERT(A._nrows == 256);
    ASSERT(A._ncols == 256);

    ASSERT(A._state == MatrixState::initialized);
}

TEST_CASE(test_JDS_constructor_initialization_size1024){
    SparseMatrix_JDS<double> A(1024, 1024);

    ASSERT(A._nrows == 1024);
    ASSERT(A._ncols == 1024);

    ASSERT(A._state == MatrixState::initialized);
}

// since most function inherited are modified, omit inheritance testing here

// test the assemble method
TEST_CASE(test_JDS_assemble){
    SparseMatrix_JDS<double> A(8, 8);

    // Assemble and basic usage test
    try {
        A.assemble();   // Should not crash
        ASSERT(true);
    } catch (...) {
        ASSERT(false);
    }
}

//test JDS bad input
TEST_CASE(test_JDS_bad_input){
    SparseMatrix_JDS<double> A(8, 8);

    // Expect that setValue out of range throws or asserts
    bool caught = false;
    try {
        A.setValue(9, 9, 10.0); // invalid indices
    } catch (...) {
        caught = true;
    }
    ASSERT(caught == true);
}


//test JDS destructor
TEST_CASE(test_JDS_destructor){
    // Create a pointer to test destructor cleanup
    auto *A = new SparseMatrix_JDS<double>(5, 5);
    delete A;

    // If no memory leaks or segfaults, test passes
    ASSERT(true);
}




TEST_SUITE(JDS_suite){
    TEST(test_JDS_constructor_initialization_size8);
    TEST(test_JDS_constructor_initialization_size64);
    TEST(test_JDS_constructor_initialization_size256);
    TEST(test_JDS_constructor_initialization_size1024);
    TEST(test_JDS_assemble);
}

int main(){
    RUN_SUITE(JDS_suite);
    return 0;
}
