/**
 * \file common.h
 * \brief Common definitions for c_traceback library
 *
 * \author Ching-Yin Ng
 */

#ifndef COMMON_H
#define COMMON_H

#include "c_traceback.h"
#include "c_traceback_errors.h"

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
    char error_message[MAX_ERROR_MESSAGE_LENGTH];
    CTB_Frame_ call_stack_frames[MAX_CALL_STACK_DEPTH];
} CTB_Error_Snapshot_;

struct CTB_Context
{
    int num_errors;
    int call_depth;
    CTB_Frame_ call_stack_frames[MAX_CALL_STACK_DEPTH];
    CTB_Error_Snapshot_ error_snapshots[MAX_ERROR_DEPTH];
};

#endif /* COMMON_H */
