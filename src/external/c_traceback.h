/**
 * \file c_traceback.h
 * \brief Library header for c_traceback library
 *
 * \author Ching-Yin Ng
 */

#ifndef C_TRACEBACK_H
#define C_TRACEBACK_H

#include "c_traceback_colors.h"
#include "c_traceback_error_codes.h"

typedef struct CTB_Context CTB_Context;

// Maximum number of traceback frames
#define MAX_TRACEBACK_FRAMES 64

/**
 * \brief Wrapper for raise_warning function.
 *
 * \param[in] error_code Error code associated with the warning.
 * \param[in] warning_msg Warning message.
 */
#define CTB_PRINT_WARNING(error_code, warning_msg)                                     \
    ctb_print_warning(__FILE__, __LINE__, __func__, error_code, warning_msg)

CTB_Context *ctb_make_context(void);
void ctb_free_context(CTB_Context *restrict context);

/**
 * \brief Print error message to stderr.
 *
 * \param[in] warning_file File where the warning occurs.
 * \param[in] warning_line Line number where the warning occurs.
 * \param[in] warning_func Function where the warning occurs.
 * \param[in] error_code Error code associated with the warning.
 * \param[in] warning_msg Warning message.
 */
void ctb_print_warning(
    const char *restrict warning_file,
    const int warning_line,
    const char *restrict warning_func,
    const int error_code,
    const char *restrict warning_msg
);

#endif