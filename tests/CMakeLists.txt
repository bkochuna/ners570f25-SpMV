include(${PROJECT_SOURCE_DIR}/cmake/spmv_add_test.cmake)

spmv_add_test(./example_unit_test.cpp)

spmv_add_test(./DEN_matvec_test.cpp)
spmv_add_test(./JDS_view_test.cpp)
spmv_add_test(./csr_matrix_tests.cpp)
