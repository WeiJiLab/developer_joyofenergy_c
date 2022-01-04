add_library(mocks)
target_sources(mocks PRIVATE clock.c metrology.c bsp.c bus_controller.cpp)
target_include_directories(mocks PUBLIC ..)
target_link_libraries(mocks PUBLIC hal)