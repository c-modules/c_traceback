/**
 * \file context.h
 * \brief Definitions of context and related data structures, and related function
 * headers for C Traceback library.
 *
 * \author Ching-Yin Ng
 */

#ifndef CONTEXT_H
#define CONTEXT_H

#include "c_traceback.h"

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

#endif /* CONTEXT_H */
