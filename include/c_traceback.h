/**
 * \file c_traceback.h
 * \brief Library header for c_traceback library
 *
 * \author Ching-Yin Ng
 */

#ifndef C_TRACEBACK_H
#define C_TRACEBACK_H

#include <stdarg.h>
#include <stdbool.h>

#include "c_traceback/color_codes.h"
#include "c_traceback/error.h"
#include "c_traceback/log_inline.h"
#include "c_traceback/trace.h"
#include "c_traceback/traceback.h"
#include "c_traceback_errors.h"

/* Traceback prefix (i.e. the string that appears before "Traceback") */
// You can put your project name here, like "MyApp"
#define CTB_TRACEBACK_PREFIX ""

// Maximum number of call stack frames
#define CTB_MAX_CALL_STACK_DEPTH 32

// Maximum number of simultaneous errors
#define CTB_MAX_NUM_ERROR 8

// Maximum length of error message
#define CTB_MAX_ERROR_MESSAGE_LENGTH 256

// Terminal width when it cannot be determined
#define CTB_DEFAULT_TERMINAL_WIDTH 80

// Output width when printing to file
#define CTB_FILE_OUTPUT_WIDTH 120

// Separator length
#define CTB_SEPARATOR_MAX_LENGTH 120
#define CTB_SEPARATOR_MIN_LENGTH 50

#endif /* C_TRACEBACK_H */
