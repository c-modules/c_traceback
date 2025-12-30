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

#include "c_traceback_colors.h"
#include "c_traceback_errors.h"
#include "config.h"

/**
 * \brief Wrapper macro for expression to automatically manage call stack frames.
 *
 * \param[in] expr The expression to be traced.
 */
#define TRACE(expr)                                                                    \
    do                                                                                 \
    {                                                                                  \
        ctb_push_call_stack_frame(__FILE__, __func__, __LINE__, #expr);                \
        (expr);                                                                        \
        ctb_pop_call_stack_frame(__FILE__, __func__, __LINE__, #expr);                 \
    } while (0)

/**
 * \brief Wrapper macro for a code block to automatically manage call stack frames.
 *
 * \param[in] ... The block of code to be traced.
 */
#define TRACE_BLOCK(...)                                                               \
    do                                                                                 \
    {                                                                                  \
        ctb_push_call_stack_frame(__FILE__, __func__, __LINE__, #__VA_ARGS__);         \
        __VA_ARGS__                                                                    \
        ctb_pop_call_stack_frame(__FILE__, __func__, __LINE__, #__VA_ARGS__);          \
    } while (0)

/**
 * \brief Wrapper for an expression. If an error occurs after the expression, jump to
 * label.
 *
 * \param[in] expr The expression to be traced.
 * \param[in] label The label to jump to on error.
 */
#define TRY_GOTO(expr, label)                                                          \
    do                                                                                 \
    {                                                                                  \
        ctb_push_call_stack_frame(__FILE__, __func__, __LINE__, #expr);                \
        (expr);                                                                        \
        ctb_pop_call_stack_frame(__FILE__, __func__, __LINE__, #expr);                 \
        if (ctb_check_error_occurred())                                                \
        {                                                                              \
            goto label;                                                                \
        }                                                                              \
    } while (0)

/**
 * \brief Wrapper for a block of code. If an error occurs after the block executes, jump
 * to label.
 *
 * \param[in] label The label to jump to on error.
 * \param[in] ...   The block of code to be traced.
 */
#define TRY_BLOCK_GOTO(label, ...)                                                     \
    do                                                                                 \
    {                                                                                  \
        ctb_push_call_stack_frame(__FILE__, __func__, __LINE__, #__VA_ARGS__);         \
        __VA_ARGS__                                                                    \
        ctb_pop_call_stack_frame(__FILE__, __func__, __LINE__, #__VA_ARGS__);          \
        if (ctb_check_error_occurred())                                                \
        {                                                                              \
            goto label;                                                                \
        }                                                                              \
    } while (0)

/**
 * \brief Wrapper for logging an error to stderr without stacktrace.
 *
 * \param[in] ctb_error The error type.
 * \param[in] msg Error message.
 * \param[in] ... Additional arguments for formatting the message.
 */
#define CTB_LOG_ERROR_INLINE(ctb_error, msg, ...)                                      \
    do                                                                                 \
    {                                                                                  \
        ctb_log_error_inline(                                                          \
            __FILE__, __LINE__, __func__, ctb_error, msg, __VA_ARGS__                  \
        );                                                                             \
    } while (0)

/**
 * \brief Wrapper for logging a warning to stderr without stacktrace.
 *
 * \param[in] ctb_warning The warning type.
 * \param[in] msg Warning message.
 * \param[in] ... Additional arguments for formatting the message.
 */
#define CTB_LOG_WARNING_INLINE(ctb_warning, msg, ...)                                  \
    do                                                                                 \
    {                                                                                  \
        ctb_log_warning_inline(                                                        \
            __FILE__, __LINE__, __func__, ctb_warning, msg, __VA_ARGS__                \
        );                                                                             \
    } while (0)

/**
 * \brief Wrapper for logging a message to stdout without stacktrace.
 *
 * \param[in] msg Warning message.
 * \param[in] ... Additional arguments for formatting the message.
 */
#define CTB_LOG_MESSAGE_INLINE(msg, ...)                                               \
    do                                                                                 \
    {                                                                                  \
        ctb_log_message_inline(__FILE__, __LINE__, __func__, msg, __VA_ARGS__);        \
    } while (0)

/**
 * \brief Push a new call stack frame.
 * \param[in] file File where the function is called.
 * \param[in] func Function name where the function is called.
 * \param[in] line Line number where the function is called.
 * \param[in] source_code Source code of the function call.
 */
void ctb_push_call_stack_frame(
    const char *file, const char *func, const int line, const char *source_code
);

/**
 * \brief Pop the top call stack frame.
 * \param[in] file File where the function is called.
 * \param[in] func Function name where the function is called.
 * \param[in] line Line number where the function is called.
 * \param[in] source_code Source code of the function call.
 */
void ctb_pop_call_stack_frame(
    const char *file, const char *func, const int line, const char *source_code
);

/**
 * \brief Log error message to stderr without stacktrace.
 *
 * \param[in] file File where the error occurs.
 * \param[in] line Line number where the error occurs.
 * \param[in] func Function where the error occurs.
 * \param[in] error The error type.
 * \param[in] msg Error message.
 * \param[in] ... Additional arguments for formatting the message.
 */
void ctb_log_error_inline(
    const char *restrict file,
    const int line,
    const char *restrict func,
    const CTB_Error error,
    const char *restrict msg,
    ...
);

/**
 * \brief Log warning message to stderr without stacktrace.
 *
 * \param[in] file File where the warning occurs.
 * \param[in] line Line number where the warning occurs.
 * \param[in] func Function where the warning occurs.
 * \param[in] warning The warning type.
 * \param[in] msg Warning message.
 * \param[in] ... Additional arguments for formatting the message.
 */
void ctb_log_warning_inline(
    const char *restrict file,
    const int line,
    const char *restrict func,
    const CTB_Warning warning,
    const char *restrict msg,
    ...
);

/**
 * \brief Log message to stdout without stacktrace.
 *
 * \param[in] file File where the message is sent.
 * \param[in] line Line number where the message is sent.
 * \param[in] func Function where the message is sent.
 * \param[in] msg Message.
 * \param[in] ... Additional arguments for formatting the message.
 */
void ctb_log_message_inline(
    const char *restrict file,
    const int line,
    const char *restrict func,
    const char *restrict msg,
    ...
);

/**
 * \brief Raise an error with the current call stack.
 *
 * \param[in] error The error type.
 * \param[in] msg Error message.
 * \param[in] ... Additional arguments for formatting the message.
 */
void ctb_raise_error(CTB_Error error, const char *restrict msg, ...);

/**
 * \brief Check if any error has occurred.
 *
 * \return true if an error has occurred, false otherwise.
 */
bool ctb_check_error_occurred(void);

/**
 * \brief Clear all recorded errors.
 */
void ctb_clear_error(void);

/**
 * \brief Log the traceback of all recorded errors to stderr.
 */
void ctb_log_error_traceback(void);

/**
 * \brief Dump the traceback of all recorded errors to stderr and clear errors.
 */
void ctb_dump_traceback(void);

#endif /* C_TRACEBACK_H */
