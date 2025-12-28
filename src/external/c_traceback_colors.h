/**
 * \file c_traceback_colors.h
 * \brief ANSI color codes for C traceback and error messages.
 *
 * \author Ching-Yin NG
 */

#ifndef C_TRACEBACK_COLORS_H
#define C_TRACEBACK_COLORS_H

// clang-format off
#define CTB_RESET_COLOR "\033[0m"                // Reset to normal text

// Error and warning
#define CTB_ERROR_BOLD_COLOR "\033[1;31m"        // Bright Red, Bold
#define CTB_ERROR_COLOR "\033[5;31m"             // Dim Red
#define CTB_WARNING_BOLD_COLOR "\033[1;33m"      // Bright Yellow, Bold
#define CTB_WARNING_COLOR "\033[5;33m"           // Dim Yellow

// Traceback/stack trace elements
#define CTB_TRACEBACK_FILE_COLOR "\033[0;36m"    // Cyan
#define CTB_TRACEBACK_LINE_COLOR "\033[0;36m"    // Cyan
#define CTB_TRACEBACK_FUNC_COLOR "\033[0;36m"    // Cyan

// Messages
#define CTB_NORMAL_BOLD_COLOR "\033[1;95m"      // Bright Purple, Bold
#define CTB_NORMAL_COLOR "\033[0;35m"           // Purple
// clang-format on

#endif /* C_TRACEBACK_COLORS_H */
