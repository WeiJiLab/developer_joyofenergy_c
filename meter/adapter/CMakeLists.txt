add_library(adapter)
target_include_directories(adapter PUBLIC ..)
target_link_libraries(adapter PUBLIC app)
add_subdirectory(protocol)