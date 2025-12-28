/**
 * \file utils.h
 * \brief Utility functions for c_traceback library.
 *
 * \author Ching-Yin Ng
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

#include "c_traceback_errors.h"

/**
 * \brief Determine if ANSI color codes should be used for the given output stream.
 *
 * \param[in] stream The output stream (e.g., stdout, stderr).
 * \return true if ANSI color codes should be used, false otherwise.
 */
bool should_use_color(FILE *stream);

/**
 * \brief Convert error type to its corresponding string representation.
 *
 * \param[in] error The error type to convert.
 * \return A constant character pointer to the string representation of the error code.
 */
const char *error_to_string(CTB_Error error);

/**
 * \brief Convert warning type to its corresponding string representation.
 *
 * \param[in] warning The warning type to convert.
 * \return A constant character pointer to the string representation of the error code.
 */
const char *warning_to_string(CTB_Warning warning);

#endif /* UTILS_H */
