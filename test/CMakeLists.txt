add_executable(c_print_test main.c)

# Link the header-only library
target_link_libraries(c_print_test PRIVATE c_print)
