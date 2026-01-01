/**
 * \file traceback.c
 * \brief Function definitions for traceback logging.
 *
 * \author Ching-Yin Ng
 */

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "c_traceback.h"
#include "internal/trace.h"
#include "internal/utils.h"

/**
 * \brief Helper function to print a single frame.
 *
 * \param[in] stream The output stream.
 * \param[in] index The index of the frame in the call stack.
 * \param[in] frame The frame to print.
 * \param[in] use_color Whether to use color in the output.
 */
static void
print_frame(FILE *stream, int index, const CTB_Frame_ *frame, bool use_color)
{
    const char *color_traceback_counter = use_color ? CTB_TRACEBACK_COUNTER_COLOR : "";
    const char *color_traceback_text = use_color ? CTB_TRACEBACK_TEXT_COLOR : "";
    const char *color_file = use_color ? CTB_TRACEBACK_FILE_COLOR : "";
    const char *color_line = use_color ? CTB_TRACEBACK_LINE_COLOR : "";
    const char *color_func = use_color ? CTB_TRACEBACK_FUNC_COLOR : "";
    const char *color_error = use_color ? CTB_ERROR_COLOR : "";
    const char *color_reset = use_color ? CTB_RESET_COLOR : "";

    const char *filename = strrchr(frame->filename, '/');

    // clang-format off
    fprintf(
        stream,
        "  %s(#%02d)%s %sFile \"%s",
        color_traceback_counter, index, color_reset,
        color_traceback_text, color_reset
    );
    // clang-format on

    if (filename)
    {
        int dir_len = (int)(filename - frame->filename) + 1;
        fprintf(
            stream,
            "%s%.*s%s",
            color_traceback_text,
            dir_len,
            frame->filename,
            color_reset
        );
        fprintf(stream, "%s%s%s", color_file, filename + 1, color_reset);
    }
    else
    {
        fprintf(stream, "%s%s%s", color_file, frame->filename, color_reset);
    }

    // clang-format off
    fprintf(
        stream,
        "%s\", line%s %s%d%s %sin%s %s%s%s:\n    %s%s%s\n",
        color_traceback_text, color_reset,
        color_line, frame->line_number, color_reset,
        color_traceback_text, color_reset,
        color_func, frame->function_name, color_reset,
        color_error, frame->source_code, color_reset
    );
    // clang-format on
}

static void print_separator(FILE *stream, bool use_color)
{
    const char *color_error = use_color ? CTB_ERROR_COLOR : "";
    const char *color_reset = use_color ? CTB_RESET_COLOR : "";

    const int terminal_width = get_terminal_width(stream);
    const int max = CTB_SEPARATOR_MAX_LENGTH;
    const int min = CTB_SEPARATOR_MIN_LENGTH;
    const int separator_width = (terminal_width < min)   ? min
                                : (terminal_width > max) ? max
                                                         : terminal_width;

    fprintf(stream, "%s", color_error);
    if (should_use_utf8(stream))
    {
        for (int i = 0; i < separator_width; i++)
        {
            fputs("\u2500", stream);
        }
    }
    else
    {
        for (int i = 0; i < separator_width; i++)
        {
            fputs("-", stream);
        }
    }
    fprintf(stream, "%s", color_reset);
}

void ctb_log_error_traceback(void)
{
    const CTB_Context *context = get_context();

    FILE *const stream = stderr;
    const bool use_color = should_use_color(stream);

    const int num_errors = context->num_errors;
    const bool num_errors_exceed_max = (num_errors > CTB_MAX_NUM_ERROR);
    const int num_errors_to_print =
        num_errors_exceed_max ? CTB_MAX_NUM_ERROR : num_errors;

    const char *color_reset = use_color ? CTB_RESET_COLOR : "";
    const char *color_error_bold = use_color ? CTB_ERROR_BOLD_COLOR : "";
    const char *color_error = use_color ? CTB_ERROR_COLOR : "";
    const char *color_traceback_text = use_color ? CTB_TRACEBACK_TEXT_COLOR : "";
    const char *color_another_exception =
        use_color ? CTB_TRACEBACK_ANOTHER_EXCEPTION_COLOR : "";

    print_separator(stream, use_color);

    if (num_errors_to_print > 0)
    {
        for (int e = 0; e < num_errors_to_print; e++)
        {
            const CTB_Error_Snapshot_ *snapshot = &context->error_snapshots[e];
            const int num_frames = snapshot->call_depth;
            const bool num_frames_exceed_max = (num_frames > CTB_MAX_CALL_STACK_DEPTH);
            const int num_frames_to_print =
                num_frames_exceed_max ? CTB_MAX_CALL_STACK_DEPTH : num_frames;

            if (CTB_TRACEBACK_PREFIX != NULL && CTB_TRACEBACK_PREFIX[0] != '\0')
            {
                fprintf(
                    stream,
                    "\n%s%s Traceback%s %s(most recent call last):%s\n",
                    color_error_bold,
                    CTB_TRACEBACK_PREFIX,
                    color_reset,
                    color_error,
                    color_reset
                );
            }
            else
            {
                fprintf(
                    stream,
                    "\n%sTraceback%s %s(most recent call last):%s\n",
                    color_error_bold,
                    color_reset,
                    color_error,
                    color_reset
                );
            }

            for (int i = 0; i < num_frames_to_print; i++)
            {
                print_frame(stream, i, &snapshot->call_stack_frames[i], use_color);
            }

            if (num_frames_exceed_max)
            {
                fprintf(
                    stream,
                    "\n      %s[... Skipped %d frames ...]%s\n\n",
                    color_traceback_text,
                    num_frames - CTB_MAX_CALL_STACK_DEPTH,
                    color_reset
                );
            }

            print_frame(stream, num_frames, &snapshot->error_frame, use_color);

            fprintf(
                stream,
                "%s%s:%s %s%s%s\n",
                color_error_bold,
                error_to_string(snapshot->error),
                color_reset,
                color_error,
                snapshot->error_message,
                color_reset
            );

            if (e < (num_errors_to_print - 1))
            {
                fprintf(
                    stream,
                    "\n%sDuring handling of the above exception, another exception "
                    "occurred:%s\n",
                    color_another_exception,
                    color_reset
                );
            }
        }
    }
    else
    {
        fputs("There is no recorded error!\n", stream);
    }

    print_separator(stream, use_color);
    fputc('\n', stream);
    fflush(stream);
}

void ctb_dump_traceback(void)
{
    ctb_log_error_traceback();
    ctb_clear_error();
}