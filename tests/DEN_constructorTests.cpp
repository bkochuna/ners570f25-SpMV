#include <SpMV.hpp>
#include <vector>

// Testing library required for testing (Always include this last!)
#include "unit_test_framework.hpp"

TEST_CASE(test_DEN_constructor_double){
    // test creation of new object
    SpMV::SparseMatrix_DEN<double> A(5,6);

    // check that the values assigned into the object are correct
    ASSERT(A.nrows() == 5);
    ASSERT(A.ncols() == 6);

    // check that object state is initialized
    ASSERT(A.getState() == SpMV::MatrixState::initialized);
}

TEST_CASE(test_DEN_destructor_double){
    // test that object can be deleted
    SpMV::SparseMatrix_DEN<double>* testing = new SpMV::SparseMatrix_DEN<double>(5,5); 
    delete testing;
    // Test will fail if the object can not be deleted
    // If test fails, a segmentation fault will occur
}

TEST_CASE(test_DEN_constructor_float){
    // test creation of new object
    SpMV::SparseMatrix_DEN<float> A(5,6);

    // check that the values assigned into the object are correct
    ASSERT(A.nrows() == 5);
    ASSERT(A.ncols() == 6);

    // check that object state is initialized
    ASSERT(A.getState() == SpMV::MatrixState::initialized);
}

TEST_CASE(test_DEN_destructor_float){
    // test that object can be deleted
    SpMV::SparseMatrix_DEN<float>* testing = new SpMV::SparseMatrix_DEN<float>(5,5); 
    delete testing;
    // Test will fail if the object can not be deleted
    // If test fails, a segmentation fault will occur
}

TEST_SUITE(DEN_con_des_suite){
    TEST(test_DEN_constructor_double);
    TEST(test_DEN_destructor_double);

    // run tests again with floats
    TEST(test_DEN_constructor_float);
    TEST(test_DEN_destructor_float);
}

int main(){
    RUN_SUITE(DEN_con_des_suite);
    return 0;
}