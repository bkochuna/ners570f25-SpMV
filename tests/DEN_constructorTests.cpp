#include "SpMV.hpp"
#include <vector>

// Testing library required for testing (Always include this last!)
#include "unit_test_framework.hpp"

TEST_CASE(test_DEN_constructor_double){
    // test creation of new object
    SpMV::SparseMatrix_DEN<double> A(5,6);

    // check that the values assigned into the object are correct
    ASSERT(A.nrows == 5);
    ASSERT(A.ncols == 6);

    // check that object state is initialized
    ASSERT(A._state == SpMV::MatrixState::initialized)
}

TEST_CASE(test_DEN_destructor_double){
    // test that object can be deleted
    SpMV::SparseMatrix_DEN<double>* testing = new SpMV::SparseMatrix_DEN<double>(5,5); 
    try {
        std::cout << "If segfault occurs now, error in DEN destructor test (probably)";
        delete testing;
    } catch (...) {
        std::cerr << "DEN destructor failed" << std::endl;
    }
    catch
    // Test will fail if the object can not be deleted
    // this test may catch some errors but will still probably casuse a segfault
    // if the test does not pass successfully
}

TEST_SUITE(DEN_con_des_suite){
    TEST(test_DEN_constructor_double);
    TEST(test_DEN_destructor_double);
}

int main(){
    RUN_SUITE(DEN_con_des_suite);
    return 0;
}
