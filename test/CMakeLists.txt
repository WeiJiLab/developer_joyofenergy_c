add_executable(endpoint_test)
target_sources(endpoint_test PRIVATE endpoint.cpp)
find_package(GTest REQUIRED)
add_subdirectory(mocks)
target_link_libraries(endpoint_test PRIVATE GTest::gmock_main meter mocks)

add_test(endpoint_test endpoint_test)
