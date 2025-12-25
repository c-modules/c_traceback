add_executable(example example.c)

target_link_libraries(example PRIVATE c_traceback::c_traceback)

if(ENABLE_SANITIZERS AND NOT MSVC)
    target_compile_options(example PRIVATE -fsanitize=address,undefined)
    target_link_options(example PRIVATE -fsanitize=address,undefined)
endif()