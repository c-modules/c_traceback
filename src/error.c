/**
 * \file error.c
 * \brief Implementation of exceptions raising and handling functions.
 *
 * \author Ching-Yin Ng
 */

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "internal/context.h"

void ctb_raise_error(
    CTB_Error error,
    const char *restrict file,
    const int line,
    const char *restrict func,
    const char *restrict msg,
    ...
)
{
    CTB_Context *context = get_context();
    (context->num_errors)++;

    const int num_errors = context->num_errors;
    if (num_errors >= 0 && num_errors < CTB_MAX_NUM_ERROR)
    {
        CTB_Error_Snapshot_ *error_snapshot =
            &(context->error_snapshots[num_errors - 1]);
        error_snapshot->error = error;
        error_snapshot->call_depth = context->call_depth;
        error_snapshot->error_frame.filename = file;
        error_snapshot->error_frame.line_number = line;
        error_snapshot->error_frame.function_name = func;
        error_snapshot->error_frame.source_code = "<Error raised here>";

        const int min_depth = (context->call_depth < CTB_MAX_CALL_STACK_DEPTH)
                                  ? context->call_depth
                                  : CTB_MAX_CALL_STACK_DEPTH;
        if (min_depth > 0)
        {
            memcpy(
                error_snapshot->call_stack_frames,
                context->call_stack_frames,
                sizeof(CTB_Frame_) * min_depth
            );
        }

        va_list args;
        va_start(args, msg);
        vsnprintf(
            error_snapshot->error_message, CTB_MAX_ERROR_MESSAGE_LENGTH, msg, args
        );
        va_end(args);
    }
}

bool ctb_check_error_occurred(void)
{
    return get_context()->num_errors > 0;
}

void ctb_clear_error(void)
{
    get_context()->num_errors = 0;
}
