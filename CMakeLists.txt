cmake_minimum_required(VERSION 3.19)
project(developer_joyofenergy_c)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

if (NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/0.17.0/conan.cmake"
                  "${CMAKE_BINARY_DIR}/conan.cmake"
                  STATUS DOWNLOAD_RESULT)
    list(GET DOWNLOAD_RESULT 0 DOWNLOAD_RESULT_CODE)
    if (NOT DOWNLOAD_RESULT_CODE EQUAL 0)
        message(FATAL_ERROR "Failed downloading! Error: ${DOWNLOAD_RESULT}.")
    endif()
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_configure(REQUIRES gtest/1.11.0
                      GENERATORS cmake_find_package)

conan_cmake_autodetect(settings)

conan_cmake_install(PATH_OR_REFERENCE .
                    BUILD missing
                    REMOTE conancenter
                    SETTINGS ${settings})

add_subdirectory(meter)
add_executable(firmware main.c)
target_link_libraries(firmware PRIVATE meter)

enable_testing()
add_subdirectory(test)
