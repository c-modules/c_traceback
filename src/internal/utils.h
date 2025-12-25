/**
 * \file utils.h
 * \brief Utility functions for c_traceback library.
 *
 * \author Ching-Yin Ng
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

/**
 * \brief Determine if ANSI color codes should be used for the given output stream.
 *
 * \param[in] stream The output stream (e.g., stdout, stderr).
 * \return true if ANSI color codes should be used, false otherwise.
 */
bool should_use_color(FILE *stream);

/**
 * \brief Convert error code to its corresponding string representation.
 *
 * \param[in] code The error code to convert.
 * \return A constant character pointer to the string representation of the error code.
 */
const char *error_code_to_string(int code);

#endif