include(${PROJECT_SOURCE_DIR}/cmake/spmv_add_test.cmake)

spmv_add_test(./example_unit_test.cpp)
spmv_add_test(./DEN_constructorTests.cpp)

spmv_add_test(./DEN_matvec_test.cpp)
spmv_add_test(./test_DEN_accessors.cpp)
spmv_add_test(./den_storage_test.cpp)