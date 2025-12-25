# --- Library ---
add_library(c_traceback STATIC
    c_traceback.c
    utils.c
)
add_library(c_traceback::c_traceback ALIAS c_traceback)

# --- Standard (c99) ---
set_target_properties(c_traceback PROPERTIES 
    C_STANDARD 99
    C_STANDARD_REQUIRED ON
    POSITION_INDEPENDENT_CODE ON
)

# --- Includes ---
target_include_directories(c_traceback 
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/internal
)

# --- Warnings ---
if(MSVC)
    target_compile_options(c_traceback PRIVATE /W4)
else()
    target_compile_options(c_traceback PRIVATE 
        -Wall -Wextra -Wpedantic -Wshadow -Wpointer-arith 
        -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes
    )
endif()

# --- Options ---
if(ENABLE_SANITIZERS)
    if(NOT MSVC)
        target_compile_options(c_traceback PRIVATE -fsanitize=address,undefined)
        target_link_options(c_traceback PRIVATE -fsanitize=address,undefined)
    endif()
endif()

# --- Definitions ---
target_compile_definitions(c_traceback PRIVATE VERSION_INFO="${PROJECT_VERSION}")

# --- Installation ---
install(TARGETS c_traceback
    EXPORT c_tracebackTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# --- Package config & version ---
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/c_tracebackConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/c_tracebackConfig.cmake.in"
    "@PACKAGE_INIT@\n\n"
    "include(\"\${CMAKE_CURRENT_LIST_DIR}/c_tracebackTargets.cmake\")\n\n"
    "check_required_components(c_traceback)\n"
)

configure_package_config_file(
    "${CMAKE_CURRENT_BINARY_DIR}/c_tracebackConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/c_tracebackConfig.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/c_traceback"
)

# --- Export targets / install cmake files ---
install(EXPORT c_tracebackTargets
    FILE c_tracebackTargets.cmake
    NAMESPACE c_traceback::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/c_traceback
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/c_tracebackConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/c_tracebackConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/c_traceback
)