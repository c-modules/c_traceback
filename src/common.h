/**
 * \file common.h
 * \brief Common definitions for c_traceback library
 *
 * \author Ching-Yin Ng
 */

#ifndef COMMON_H
#define COMMON_H

#include "c_traceback.h"

typedef struct CTB_Frame_
{
    const char *restrict filename;
    const char *restrict function_name;
    const int line_number;
} CTB_Frame_;

typedef struct CTB_Error_
{
    const char *restrict message;
    int error_code;
} CTB_Error_;

struct CTB_Context
{
    int num_errors;
    CTB_Error_ errors[MAX_TRACEBACK_FRAMES];
    CTB_Frame_ stack_frames[MAX_TRACEBACK_FRAMES];
};

#endif