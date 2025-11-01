add_executable(mwe mwe.cpp)
target_include_directories(mwe PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(mwe PRIVATE spmv)
