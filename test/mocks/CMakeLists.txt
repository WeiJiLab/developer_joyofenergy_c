add_library(mocks)
target_sources(mocks PRIVATE clock.c endpoint.c metrology.c bsp.c)
target_include_directories(mocks PUBLIC ..)
target_link_libraries(mocks PUBLIC hal)