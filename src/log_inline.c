/**
 * \file error.c
 * \brief Function definitions for inline logging
 *
 * \author Ching-Yin Ng
 */

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "c_traceback.h"
#include "c_traceback_colors.h"
#include "c_traceback_errors.h"
#include "internal/utils.h"

static void ctb_log_inline(
    FILE *stream,
    const char *header_color,
    const char *message_color,
    const char *restrict file_address,
    const int line,
    const char *restrict func,
    const char *restrict header,
    const char *restrict msg,
    va_list args
)
{
    if (should_use_color(stream))
    {
        const char *filename = strrchr(file_address, '/');

        // clang-format off
        fprintf(
            stream,
            "%s%s:%s %sFile \"%s",
            header_color, header, CTB_RESET_COLOR,
            CTB_TRACEBACK_TEXT_COLOR, CTB_RESET_COLOR
        );
        // clang-format on

        if (filename)
        {
            int dir_len = (int)(filename - file_address) + 1;
            fprintf(
                stream,
                "%s%.*s%s",
                CTB_TRACEBACK_TEXT_COLOR,
                dir_len,
                file_address,
                CTB_RESET_COLOR
            );
            fprintf(
                stream,
                "%s%s%s",
                CTB_TRACEBACK_FILE_COLOR,
                filename + 1,
                CTB_RESET_COLOR
            );
        }
        else
        {
            fprintf(
                stream,
                "%s%s%s",
                CTB_TRACEBACK_FILE_COLOR,
                file_address,
                CTB_RESET_COLOR
            );
        }

        fprintf(
            stream,
            "%s\", line%s %s%d%s %sin%s %s%s%s:\n   %s",
            CTB_TRACEBACK_TEXT_COLOR,
            CTB_RESET_COLOR,
            CTB_TRACEBACK_LINE_COLOR,
            line,
            CTB_RESET_COLOR,
            CTB_TRACEBACK_TEXT_COLOR,
            CTB_RESET_COLOR,
            CTB_TRACEBACK_FUNC_COLOR,
            func,
            CTB_RESET_COLOR,
            message_color
        );
        vfprintf(stream, msg, args);
        fprintf(stream, "%s\n", CTB_RESET_COLOR);
        fflush(stream);
    }
    else
    {
        fprintf(
            stream,
            "%s: File \"%s\", line %d in %s:\n    ",
            header,
            file_address,
            line,
            func
        );
        vfprintf(stream, msg, args);
        fputc('\n', stream);
        fflush(stream);
    }
}

void ctb_log_error_inline(
    const char *restrict file,
    const int line,
    const char *restrict func,
    const CTB_Error error,
    const char *restrict msg,
    ...
)
{
    FILE *stream = stderr;
    va_list args;
    va_start(args, msg);
    ctb_log_inline(
        stream,
        CTB_ERROR_BOLD_COLOR,
        CTB_ERROR_COLOR,
        file,
        line,
        func,
        error_to_string(error),
        msg,
        args
    );
    va_end(args);
}

void ctb_log_warning_inline(
    const char *restrict file,
    const int line,
    const char *restrict func,
    const CTB_Warning warning,
    const char *restrict msg,
    ...
)
{
    FILE *stream = stderr;
    va_list args;
    va_start(args, msg);
    ctb_log_inline(
        stream,
        CTB_WARNING_BOLD_COLOR,
        CTB_WARNING_COLOR,
        file,
        line,
        func,
        warning_to_string(warning),
        msg,
        args
    );
    va_end(args);
}

void ctb_log_message_inline(
    const char *restrict file,
    const int line,
    const char *restrict func,
    const char *restrict msg,
    ...
)
{
    FILE *stream = stdout;
    va_list args;
    va_start(args, msg);
    ctb_log_inline(
        stream,
        CTB_NORMAL_BOLD_COLOR,
        CTB_NORMAL_COLOR,
        file,
        line,
        func,
        "Message",
        msg,
        args
    );
    va_end(args);
}
