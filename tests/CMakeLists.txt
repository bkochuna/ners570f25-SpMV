include(${PROJECT_SOURCE_DIR}/cmake/spmv_add_test.cmake)

spmv_add_test(./example_unit_test.cpp)
spmv_add_test(./COO_constructor_destructor_test.cpp)
spmv_add_test(./DEN_constructorTests.cpp)
spmv_add_test(./test_SparseMatrix_JDS.cpp)
spmv_add_test(./JDS_view_test.cpp)
spmv_add_test(./csr_matrix_tests.cpp)
