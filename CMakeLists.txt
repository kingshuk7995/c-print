cmake_minimum_required(VERSION 3.14)
project(c_print VERSION 1.0 LANGUAGES C)

add_library(c_print INTERFACE)
target_include_directories(c_print INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_compile_features(c_print INTERFACE c_std_11)

install(TARGETS c_print EXPORT c_printTargets)
install(DIRECTORY include/ DESTINATION include)
install(EXPORT c_printTargets
    FILE c_printTargets.cmake
    NAMESPACE c_print::
    DESTINATION lib/cmake/c_print
)

add_subdirectory(test)
