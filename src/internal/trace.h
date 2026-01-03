/**
 * \file trace.h
 * \brief Definitions of context and related data structures, and related function
 * headers for C Traceback library.
 *
 * \author Ching-Yin Ng
 */

#ifndef C_TRACEBACK_INTERNAL_TRACE_H
#define C_TRACEBACK_INTERNAL_TRACE_H

#include "c_traceback.h"

#ifndef ctb_thread_local
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
/* C23 made 'thread_local' a standard keyword; map to it. */
#define ctb_thread_local thread_local
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
/* C11 provides the _Thread_local storage-class specifier.
   Map ctb_thread_local to _Thread_local for thread-local storage. */
#define ctb_thread_local _Thread_local
#elif defined(__GNUC__) || defined(__clang__) || defined(__MINGW32__)
/* GCC, Clang, and MinGW use __thread for C99 */
#define ctb_thread_local __thread
#elif defined(_MSC_VER)
/* MSVC (Visual Studio) uses __declspec(thread) */
#define ctb_thread_local __declspec(thread)
#else
#error "Cannot define thread_local for this compiler/standard."
#endif
#endif

typedef struct CTB_Frame_
{
    int line_number;
    const char *restrict filename;
    const char *restrict function_name;
    const char *restrict source_code;
} CTB_Frame_;

typedef struct CTB_Error_Snapshot_
{
    CTB_Error error;
    int call_depth;
    CTB_Frame_ error_frame;
    char error_message[CTB_MAX_ERROR_MESSAGE_LENGTH];
    CTB_Frame_ call_stack_frames[CTB_MAX_CALL_STACK_DEPTH];
} CTB_Error_Snapshot_;

typedef struct CTB_Context
{
    int num_errors;
    int call_depth;
    CTB_Frame_ call_stack_frames[CTB_MAX_CALL_STACK_DEPTH];
    CTB_Error_Snapshot_ error_snapshots[CTB_MAX_NUM_ERROR];
} CTB_Context;

/**
 * \brief Get the global C Traceback context.
 *
 * \return Pointer to the global C Traceback context.
 */
CTB_Context *get_context(void);

#endif /* C_TRACEBACK_INTERNAL_TRACE_H */
