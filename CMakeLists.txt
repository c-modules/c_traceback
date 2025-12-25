cmake_minimum_required(VERSION 3.14)

project(c_traceback
    VERSION 0.0.1
    LANGUAGES C
    DESCRIPTION "Colorful, lightweight tracebacks in C"
)

# --- Dependencies & Modules ---
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# --- Options ---
# Defined here so they are visible to all subdirectories
option(ENABLE_SANITIZERS "Enable address and undefined sanitizers" OFF)
option(BUILD_EXAMPLES "Build example executables" ON)

# --- Main sources directory (the second parameter sets the output directory name to c_traceback) ---
add_subdirectory(src c_traceback)

if(BUILD_EXAMPLES)
    message(STATUS "Building examples is enabled")
    add_subdirectory(examples)
endif()
