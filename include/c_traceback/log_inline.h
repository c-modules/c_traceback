/**
 * \file log_inline.h
 * \brief Header file for inline logging functions.
 *
 * \author Ching-Yin Ng
 */

#ifndef LOG_INLINE_H
#define LOG_INLINE_H

#include "c_traceback_errors.h"

/**
 * \brief Wrapper for logging an error to stderr without stacktrace.
 *
 * \param[in] ctb_error The error type.
 * \param[in] msg Error message.
 */
#define LOG_ERROR_INLINE(ctb_error, msg)                                               \
    do                                                                                 \
    {                                                                                  \
        ctb_log_error_inline(__FILE__, __LINE__, __func__, ctb_error, msg);            \
    } while (0)

/**
 * \brief Wrapper for logging a warning to stderr without stacktrace.
 *
 * \param[in] ctb_warning The warning type.
 * \param[in] msg Warning message.
 */
#define LOG_WARNING_INLINE(ctb_warning, msg)                                           \
    do                                                                                 \
    {                                                                                  \
        ctb_log_warning_inline(__FILE__, __LINE__, __func__, ctb_warning, msg);        \
    } while (0)

/**
 * \brief Wrapper for logging a message to stdout without stacktrace.
 *
 * \param[in] msg Message.
 */
#define LOG_MESSAGE_INLINE(msg)                                                        \
    do                                                                                 \
    {                                                                                  \
        ctb_log_message_inline(__FILE__, __LINE__, __func__, msg);                     \
    } while (0)

/**
 * \brief Wrapper for logging an error with formatted message to stderr without
 * stacktrace.
 *
 * \param[in] ctb_error The error type.
 * \param[in] msg Error message.
 * \param[in] ... Additional arguments for formatting the message.
 */
#define LOG_ERROR_INLINE_FMT(ctb_error, msg, ...)                                      \
    do                                                                                 \
    {                                                                                  \
        ctb_log_error_inline_fmt(                                                      \
            __FILE__, __LINE__, __func__, ctb_error, msg, __VA_ARGS__                  \
        );                                                                             \
    } while (0)

/**
 * \brief Wrapper for logging a warning with formatted message to stderr without
 * stacktrace.
 *
 * \param[in] ctb_warning The warning type.
 * \param[in] msg Warning message.
 * \param[in] ... Additional arguments for formatting the message.
 */
#define LOG_WARNING_INLINE_FMT(ctb_warning, msg, ...)                                  \
    do                                                                                 \
    {                                                                                  \
        ctb_log_warning_inline_fmt(                                                    \
            __FILE__, __LINE__, __func__, ctb_warning, msg, __VA_ARGS__                \
        );                                                                             \
    } while (0)

/**
 * \brief Wrapper for logging a formatted message to stdout without stacktrace.
 *
 * \param[in] msg Warning message.
 * \param[in] ... Additional arguments for formatting the message.
 */
#define LOG_MESSAGE_INLINE_FMT(msg, ...)                                               \
    do                                                                                 \
    {                                                                                  \
        ctb_log_message_inline_fmt(__FILE__, __LINE__, __func__, msg, __VA_ARGS__);    \
    } while (0)

/**
 * \brief Log error with message to stderr without stacktrace.
 *
 * \param[in] file File where the error occurs.
 * \param[in] line Line number where the error occurs.
 * \param[in] func Function where the error occurs.
 * \param[in] error The error type.
 * \param[in] msg Error message.
 */
void ctb_log_error_inline(
    const char *restrict file,
    const int line,
    const char *restrict func,
    const CTB_Error error,
    const char *restrict msg
);

/**
 * \brief Log warning with message to stderr without stacktrace.
 *
 * \param[in] file File where the warning occurs.
 * \param[in] line Line number where the warning occurs.
 * \param[in] func Function where the warning occurs.
 * \param[in] warning The warning type.
 * \param[in] msg Warning message.
 */
void ctb_log_warning_inline(
    const char *restrict file,
    const int line,
    const char *restrict func,
    const CTB_Warning warning,
    const char *restrict msg
);

/**
 * \brief Log message to stdout without stacktrace.
 *
 * \param[in] file File where the message is sent.
 * \param[in] line Line number where the message is sent.
 * \param[in] func Function where the message is sent.
 * \param[in] msg Message.
 */
void ctb_log_message_inline(
    const char *restrict file,
    const int line,
    const char *restrict func,
    const char *restrict msg
);

/**
 * \brief Log error with formatted message to stderr without stacktrace.
 *
 * \param[in] file File where the error occurs.
 * \param[in] line Line number where the error occurs.
 * \param[in] func Function where the error occurs.
 * \param[in] error The error type.
 * \param[in] msg Error message.
 * \param[in] ... Additional arguments for formatting the message.
 */
void ctb_log_error_inline_fmt(
    const char *restrict file,
    const int line,
    const char *restrict func,
    const CTB_Error error,
    const char *restrict msg,
    ...
);

/**
 * \brief Log warning with formatted message to stderr without stacktrace.
 *
 * \param[in] file File where the warning occurs.
 * \param[in] line Line number where the warning occurs.
 * \param[in] func Function where the warning occurs.
 * \param[in] warning The warning type.
 * \param[in] msg Warning message.
 * \param[in] ... Additional arguments for formatting the message.
 */
void ctb_log_warning_inline_fmt(
    const char *restrict file,
    const int line,
    const char *restrict func,
    const CTB_Warning warning,
    const char *restrict msg,
    ...
);

/**
 * \brief Log formatted message to stdout without stacktrace.
 *
 * \param[in] file File where the message is sent.
 * \param[in] line Line number where the message is sent.
 * \param[in] func Function where the message is sent.
 * \param[in] msg Message.
 * \param[in] ... Additional arguments for formatting the message.
 */
void ctb_log_message_inline_fmt(
    const char *restrict file,
    const int line,
    const char *restrict func,
    const char *restrict msg,
    ...
);

#endif // LOG_INLINE_H