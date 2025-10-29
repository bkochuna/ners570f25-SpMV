# Use the project-provided test macro for the example test
include(${PROJECT_SOURCE_DIR}/cmake/spmv_add_test.cmake)
spmv_add_test(./example_unit_test.cpp)

# Register your ELL test manually using the same framework
add_executable(test_ell_view test_ell_view.cpp)
target_link_libraries(test_ell_view PRIVATE spmv)
add_test(NAME test_ell_view COMMAND test_ell_view)
