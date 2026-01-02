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

/**
 * \brief Helper function to print a horizontal rule.
 *
 * \param[in] stream The output stream.
 * \param[in] use_color Whether to use color in the output.
 * \param[in] color The color code to use for the horizontal rule.
 */
static void print_hrule(FILE *stream, bool use_color, const char *restrict color)
{
    const int terminal_width = get_terminal_width(stream);
    const int max = CTB_HRULE_MAX_WIDTH;
    const int min = CTB_HRULE_MIN_WIDTH;
    const int separator_width = (terminal_width < min)   ? min
                                : (terminal_width > max) ? max
                                                         : terminal_width;
    const char *dash = should_use_utf8(stream) ? "\u2500" : "-";

    if (use_color)
    {
        fprintf(stream, "%s", color);
    }

    for (int i = 0; i < separator_width; i++)
    {
        fputs(dash, stream);
    }

    if (use_color)
    {
        fprintf(stream, "%s", CTB_RESET_COLOR);
    }

    fputs("\n", stream);
}

/**
 * \brief Helper function to print a horizontal rule with a header.
 *
 * \param[in] stream The output stream.
 * \param[in] use_color Whether to use color in the output.
 * \param[in] color The color code to use for the horizontal rule.
 * \param[in] header The header text to display in the middle of the rule.
 */
static void print_hrule_with_header(
    FILE *stream,
    bool use_color,
    const char *restrict color,
    const char *restrict header
)
{
    const int terminal_width = get_terminal_width(stream);
    const int max = CTB_HRULE_MAX_WIDTH;
    const int min = CTB_HRULE_MIN_WIDTH;
    const int separator_width = (terminal_width < min)   ? min
                                : (terminal_width > max) ? max
                                                         : terminal_width;
    const char *dash = should_use_utf8(stream) ? "\u2500" : "-";
    const int header_width = (int)strlen(header);
    int left_width = 0;
    int right_width = 0;

    if (header_width > 0)
    {
        const int padding = 2;
        const int available_space = separator_width - header_width - padding;

        if (available_space > 0)
        {
            left_width = available_space / 2;
            right_width = available_space - left_width;
        }
        else
        {
            left_width = 2;
            right_width = 2;
        }
    }
    else
    {
        // No label, print full line
        left_width = separator_width;
        right_width = 0;
    }

    if (use_color)
    {
        fprintf(stream, "%s", color);
    }

    for (int i = 0; i < left_width; i++)
    {
        fputs(dash, stream);
    }

    fprintf(stream, " %s ", header);

    for (int i = 0; i < right_width; i++)
    {
        fputs(dash, stream);
    }

    if (use_color)
    {
        fprintf(stream, "%s", CTB_RESET_COLOR);
    }

    fputs("\n", stream);
}

/**
 * \brief Helper function to print bold text.
 *
 * \param[in] stream The output stream.
 * \param[in] use_color Whether to use color in the output.
 * \param[in] color The color code to use for the bold text.
 * \param[in] text The text to print in bold.
 */
static void
print_bold(FILE *stream, bool use_color, const char *color, const char *text)
{
    if (use_color)
    {
        fprintf(stream, "%s%s%s", color, text, CTB_RESET_COLOR);
    }
    else
    {
        fputs(text, stream);
    }
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
        use_color ? CTB_TRACEBACK_ANOTHER_EXCEPTION_TEXT_COLOR : "";

    print_hrule(stream, use_color, CTB_ERROR_COLOR);

    if (num_errors_to_print > 0)
    {
        for (int e = 0; e < num_errors_to_print; e++)
        {
            const CTB_Error_Snapshot_ *snapshot = &context->error_snapshots[e];
            const int num_frames = snapshot->call_depth;
            const bool num_frames_exceed_max = (num_frames > CTB_MAX_CALL_STACK_DEPTH);
            const int num_frames_to_print =
                num_frames_exceed_max ? CTB_MAX_CALL_STACK_DEPTH : num_frames;

            if (CTB_TRACEBACK_HEADER != NULL && CTB_TRACEBACK_HEADER[0] != '\0')
            {
                fprintf(
                    stream,
                    "%s%s%s %s(most recent call last):%s\n",
                    color_error_bold,
                    CTB_TRACEBACK_HEADER,
                    color_reset,
                    color_error,
                    color_reset
                );
            }
            else
            {
                fprintf(
                    stream,
                    "%sTraceback%s %s(most recent call last):%s\n",
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
                    "occurred:%s\n\n",
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

    print_hrule(stream, use_color, CTB_ERROR_COLOR);
    fflush(stream);
}

void ctb_dump_traceback(void)
{
    ctb_log_error_traceback();
    ctb_clear_error();
}

/**
 * \brief Helper function to print the left column of the compilation info.
 *
 * \param[in] stream The output stream.
 * \param[in] use_color Whether to use color in the output.
 * \param[in] logo_lines The lines of the logo to print.
 * \param[in] logo_height The height of the logo.
 * \param[in] logo_width The width of the logo.
 * \param[in] left_padding The left padding before the logo.
 * \param[in] gutter The gutter space after the logo.
 * \param[in] line_idx The current line index to print.
 */
static void print_compilation_info_left_column(
    FILE *stream,
    const bool use_color,
    const char *restrict logo_lines[],
    const int logo_height,
    const int logo_width,
    const int left_padding,
    const int gutter,
    int line_idx
)
{
    const char *color_reset = use_color ? CTB_RESET_COLOR : "";
    const char *color_theme_bold = use_color ? CTB_THEME_BOLD_COLOR : "";

    if (line_idx < logo_height)
    {
        fprintf(
            stream,
            "%*s%s%s%s%*s",
            left_padding,
            "",
            color_theme_bold,
            logo_lines[line_idx],
            color_reset,
            gutter,
            ""
        );
    }
    else
    {
        fprintf(stream, "%*s", left_padding + logo_width + gutter, "");
    }
}

void ctb_print_compilation_info(void)
{
    FILE *const stream = stdout;
    const bool use_color = should_use_color(stream);
    const char *dash = should_use_utf8(stream) ? "\u2500" : "-";
    const char *color_reset = use_color ? CTB_RESET_COLOR : "";
    const char *color_error_bold = use_color ? CTB_ERROR_BOLD_COLOR : "";
    const char *color_error = use_color ? CTB_ERROR_COLOR : "";
    const char *color_traceback_text = use_color ? CTB_TRACEBACK_TEXT_COLOR : "";
    // const char *color_another_exception =
    //     use_color ? CTB_TRACEBACK_ANOTHER_EXCEPTION_TEXT_COLOR : "";

    // int terminal_width;
    // {
    //     const int temp = get_terminal_width(stream);
    //     const int max = CTB_HRULE_MAX_WIDTH;
    //     const int min = CTB_HRULE_MIN_WIDTH;
    //     terminal_width = (temp < min) ? min : (temp > max) ? max : temp;
    // }

    const char *logo_lines[] = {
        "    %%%%%%%%%%%%    ",
        "  %%%%%%%%%%%%%%%%  ",
        " %%%%%%%%%%%%%%%%%% ",
        "%%%%%%%%%%*  %%%%%%%",
        "%%%%%%%*     %%%%%%%",
        "%%%%%*       %%%%%%%",
        "%%%%%%%*     %%%%%%%",
        "%%%%%%%%%%*  %%%%%%%",
        " %%%%%%%%%%%%%%%%%% ",
        "  %%%%%%%%%%%%%%%%  ",
        "    %%%%%%%%%%%%    "
    };

    const int logo_height = sizeof(logo_lines) / sizeof(logo_lines[0]);
    const int logo_width = strlen(logo_lines[0]);
    const int left_padding = 2;
    const int gutter = 4;
    // const int left_col_total = left_padding + logo_width + gutter;
    // const int right_col_max_width = terminal_width - left_col_total;

    print_hrule_with_header(
        stream, use_color, CTB_THEME_COLOR, "C Traceback Compilation Info"
    );

    // Sorry, but right now its hard-coded, and without
    // terminal width calculation for wrapping.
    int current_line = 0;
    const int total_items = 14;
    for (int i = 0; i < total_items; i++)
    {
        print_compilation_info_left_column(
            stream,
            use_color,
            logo_lines,
            logo_height,
            logo_width,
            left_padding,
            gutter,
            current_line
        );

        if (i == 0)
        {
            /* Version */
            print_bold(
                stream, use_color, CTB_THEME_BOLD_COLOR, "C Traceback Version: "
            );
            fputs(CTB_VERSION, stream);
        }
        else if (i == 1)
        {
            /* OS information */
            print_bold(stream, use_color, CTB_THEME_BOLD_COLOR, "Operating System: ");
#ifdef _WIN32
            fputs("Windows", stream);
#elif __APPLE__
            fputs("MacOS", stream);
#elif __linux__
            fputs("Linux", stream);
#else
            fputs("Unknown", stream);
#endif
        }
        else if (i == 2)
        {
            /* Build date and time */
            print_bold(stream, use_color, CTB_THEME_BOLD_COLOR, "Build Date: ");
            fprintf(stream, "%s %s", __DATE__, __TIME__);
        }
        else if (i == 3)
        {
            /* Compiler information */
            print_bold(stream, use_color, CTB_THEME_BOLD_COLOR, "Compiler: ");
#ifdef _MSC_VER
            fprintf(stream, "MSVC (version: %d)", _MSC_VER);
#elif defined(__clang__)
            fprintf(stream, "Clang (version: %d)", __clang_major__);
#elif defined(__GNUC__)
            fprintf(stream, "GCC (version: %d)", __GNUC__);
#else
            fputs("Unknown", stream);
#endif
        }

        /* Config */
        else if (i == 5)
        {
            print_bold(stream, use_color, CTB_THEME_BOLD_COLOR, "Config");
        }
        else if (i == 6)
        {
            for (int d = 0; d < 6; d++)
            {
                fputs(dash, stream);
            }
        }
        else if (i == 7)
        {
            print_bold(
                stream, use_color, CTB_THEME_BOLD_COLOR, "Max Call Stack Depth: "
            );
            fprintf(stream, "%d", CTB_MAX_CALL_STACK_DEPTH);
        }
        else if (i == 8)
        {
            print_bold(
                stream, use_color, CTB_THEME_BOLD_COLOR, "Max Error Message Length: "
            );
            fprintf(stream, "%d", CTB_MAX_ERROR_MESSAGE_LENGTH);
        }
        else if (i == 9)
        {
            print_bold(
                stream, use_color, CTB_THEME_BOLD_COLOR, "Max Number of Errors: "
            );
            fprintf(stream, "%d", CTB_MAX_NUM_ERROR);
        }
        else if (i == 10)
        {
            print_bold(
                stream, use_color, CTB_THEME_BOLD_COLOR, "Default Terminal Width: "
            );
            fprintf(stream, "%d", CTB_DEFAULT_TERMINAL_WIDTH);
        }
        else if (i == 11)
        {
            print_bold(stream, use_color, CTB_THEME_BOLD_COLOR, "Default File Width: ");
            fprintf(stream, "%d", CTB_DEFAULT_FILE_WIDTH);
        }
        else if (i == 12)
        {
            print_bold(
                stream, use_color, CTB_THEME_BOLD_COLOR, "Horizontal Rule Max Width: "
            );
            fprintf(stream, "%d", CTB_HRULE_MAX_WIDTH);
        }
        else if (i == 13)
        {
            print_bold(
                stream, use_color, CTB_THEME_BOLD_COLOR, "Horizontal Rule Min Width: "
            );
            fprintf(stream, "%d", CTB_HRULE_MIN_WIDTH);
        }

        fprintf(stream, "\n");
        current_line++;
    }

    while (current_line < logo_height)
    {
        print_compilation_info_left_column(
            stream,
            use_color,
            logo_lines,
            logo_height,
            logo_width,
            left_padding,
            gutter,
            current_line
        );
        fprintf(stream, "\n");
        current_line++;
    }

    /* Sample inline logging */
    fputs("\n", stream);
    print_bold(stream, use_color, CTB_THEME_BOLD_COLOR, "Inline logging (example)\n");
    for (int d = 0; d < 24; d++)
    {
        fputs(dash, stream);
    }
    fputs("\n", stream);
    LOG_ERROR_INLINE(CTB_BASE_EXCEPTION, "Sample error for compilation info");
    LOG_WARNING_INLINE(CTB_USER_WARNING, "Sample warning for compilation info");
    LOG_MESSAGE_INLINE("Sample info for compilation info");

    /* Sample Traceback */
    const int num_examples = 3;
    const CTB_Frame_ example_frames[3] = {
        {10, "example/example.c", "main", "hello_world();"},
        {25, "example/hello_world.c", "check_terminal", "data = compute(data)"},
        {50, "example/libs/utils.c", "compute", "recursion()"}
    };

    const CTB_Frame_ error_frame = {
        75, "example/libs/utils.c", "recursion", "<error raised here>"
    };

    fputs("\n", stream);
    print_bold(stream, use_color, CTB_THEME_BOLD_COLOR, "Traceback (example)\n");
    for (int d = 0; d < 19; d++)
    {
        fputs(dash, stream);
    }
    fputs("\n", stream);

    if (CTB_TRACEBACK_HEADER != NULL && CTB_TRACEBACK_HEADER[0] != '\0')
    {
        fprintf(
            stream,
            "%s%s%s %s(most recent call last):%s\n",
            color_error_bold,
            CTB_TRACEBACK_HEADER,
            color_reset,
            color_error,
            color_reset
        );
    }
    else
    {
        fprintf(
            stream,
            "%sTraceback%s %s(most recent call last):%s\n",
            color_error_bold,
            color_reset,
            color_error,
            color_reset
        );
    }

    for (int i = 0; i < num_examples; i++)
    {
        print_frame(stream, i, &example_frames[i], use_color);
    }

    fprintf(
        stream,
        "\n      %s[... Skipped %d frames ...]%s\n\n",
        color_traceback_text,
        123,
        color_reset
    );

    print_frame(stream, 127, &error_frame, use_color);
    fprintf(
        stream,
        "%s%s:%s %s%s%s\n",
        color_error_bold,
        error_to_string(CTB_EXCEPTION),
        color_reset,
        color_error,
        "Something went wrong!",
        color_reset
    );

    print_hrule_with_header(stream, use_color, CTB_THEME_COLOR, "END");
    fflush(stream);
}
