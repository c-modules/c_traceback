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

typedef struct
{
    const char *reset;
    const char *bold;
    const char *error;
    const char *error_bold;
    const char *tb_text;
    const char *tb_counter;
    const char *tb_file;
    const char *tb_line;
    const char *tb_func;
    const char *tb_another_exception;
    const char *theme_bold;
    const char *theme;
} Theme;

static const char *LOGO_LINES[] = {
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

/**
 * \brief Populates the Theme struct based on color availability.
 *
 * \param[in] use_color Whether to use color in the output.
 * \return The populated Theme struct.
 */
static Theme get_theme(bool use_color)
{
    if (!use_color)
    {
        return (Theme){"", "", "", "", "", "", "", "", "", "", "", ""};
    }

    return (Theme
    ){.reset = CTB_RESET_COLOR,
      .bold = CTB_THEME_BOLD_COLOR,
      .error = CTB_ERROR_COLOR,
      .error_bold = CTB_ERROR_BOLD_COLOR,
      .tb_text = CTB_TRACEBACK_TEXT_COLOR,
      .tb_counter = CTB_TRACEBACK_COUNTER_COLOR,
      .tb_file = CTB_TRACEBACK_FILE_COLOR,
      .tb_line = CTB_TRACEBACK_LINE_COLOR,
      .tb_func = CTB_TRACEBACK_FUNC_COLOR,
      .tb_another_exception = CTB_TRACEBACK_ANOTHER_EXCEPTION_TEXT_COLOR,
      .theme_bold = CTB_THEME_BOLD_COLOR,
      .theme = CTB_THEME_COLOR};
}

/**
 * \brief Returns the appropriate dash character (UTF-8 or ASCII).
 *
 * \param[in] stream The output stream.
 * \return The dash character as a string.
 */
static const char *get_dash(FILE *stream)
{
    return should_use_utf8(stream) ? "\u2500" : "-";
}

/**
 * \brief Helper function to print a single frame.
 *
 * \param[in] stream The output stream.
 * \param[in] index The index of the frame in the call stack.
 * \param[in] frame The frame to print.
 * \param[in] use_color Whether to use color in the output.
 */
static void
print_frame(FILE *stream, int index, const CTB_Frame_ *frame, const Theme *theme)
{
    const int dir_len = get_parent_path_length(frame->filename);

    // clang-format off
    fprintf(
        stream,
        "  %s(#%02d)%s %sFile \"%s",
        theme->tb_counter, index, theme->reset,
        theme->tb_text, theme->reset
    );
    // clang-format on

    fprintf(stream, "%s%.*s%s", theme->tb_text, dir_len, frame->filename, theme->reset);
    fprintf(stream, "%s%s%s", theme->tb_file, frame->filename + dir_len, theme->reset);

    // clang-format off
    fprintf(
        stream,
        "%s\", line%s %s%d%s %sin%s %s%s%s:\n    %s%s%s\n",
        theme->tb_text, theme->reset,
        theme->tb_line, frame->line_number, theme->reset,
        theme->tb_text, theme->reset,
        theme->tb_func, frame->function_name, theme->reset,
        theme->error, frame->source_code, theme->reset
    );
    // clang-format on
}

/**
 * \brief Helper function to print a horizontal rule with optional headers.
 *
 * \param[in] stream The output stream.
 * \param[in] use_color Whether to use color in the output.
 * \param[in] color_code The color code to use for the horizontal rule.
 * \param[in] header The header text to display in the middle of the rule.
 */
static void print_hrule_internal(
    FILE *stream,
    const bool use_color,
    const char *restrict color_code,
    const char *restrict header
)
{
    const int terminal_width = get_terminal_width(stream);
    const int max = CTB_HRULE_MAX_WIDTH;
    const int min = CTB_HRULE_MIN_WIDTH;

    int hrule_width = terminal_width;
    if (hrule_width < min)
    {
        hrule_width = min;
    }
    else if (hrule_width > max)
    {
        hrule_width = max;
    }

    const char *dash = get_dash(stream);
    const char *reset = use_color ? CTB_RESET_COLOR : "";
    const char *color = (use_color && color_code) ? color_code : "";

    int header_len = (header) ? (int)strlen(header) : 0;
    int left_width = hrule_width;
    int right_width = 0;

    if (header_len > 0)
    {
        const int padding = 2;
        const int available_space = hrule_width - header_len - padding;

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

    fprintf(stream, "%s", color);

    for (int i = 0; i < left_width; i++)
    {
        fputs(dash, stream);
    }

    if (header_len > 0)
    {
        fprintf(stream, " %s ", header);
    }

    for (int i = 0; i < right_width; i++)
    {
        fputs(dash, stream);
    }
    fprintf(stream, "%s\n", reset);
}

/**
 * \brief Print a horizontal rule without a header.
 *
 * \param[in] stream The output stream.
 * \param[in] use_color Whether to use color in the output.
 * \param[in] color_code The color code to use for the horizontal rule.
 */
static void
print_hrule(FILE *stream, const bool use_color, const char *restrict color_code)
{
    print_hrule_internal(stream, use_color, color_code, NULL);
}

/**
 * \brief Print a horizontal rule with a header.
 *
 * \param[in] stream The output stream.
 * \param[in] use_color Whether to use color in the output.
 * \param[in] color_code The color code to use for the horizontal rule.
 * \param[in] header The header text to display in the middle of the rule.
 */
static void print_hrule_with_header(
    FILE *stream,
    const bool use_color,
    const char *restrict color_code,
    const char *restrict header
)
{
    print_hrule_internal(stream, use_color, color_code, header);
}

void ctb_log_error_traceback(void)
{
    const CTB_Context *context = get_context();
    FILE *const stream = stderr;
    const bool use_color = should_use_color(stream);
    const Theme theme = get_theme(use_color);

    const int num_errors = context->num_errors;
    const int num_errors_to_print =
        (num_errors > CTB_MAX_NUM_ERROR) ? CTB_MAX_NUM_ERROR : num_errors;

    print_hrule(stream, use_color, CTB_ERROR_COLOR);

    if (num_errors_to_print <= 0)
    {
        fputs("There is no recorded error!\n", stream);
        print_hrule(stream, use_color, CTB_ERROR_COLOR);
        fflush(stream);
        return;
    }

    for (int e = 0; e < num_errors_to_print; e++)
    {
        const CTB_Error_Snapshot_ *snapshot = &context->error_snapshots[e];
        const int num_frames = snapshot->call_depth;
        const bool stack_frames_exceed_max = (num_frames > CTB_MAX_CALL_STACK_DEPTH);
        const int num_frames_to_print =
            stack_frames_exceed_max ? CTB_MAX_CALL_STACK_DEPTH : num_frames;

        /* Print Header */
        const char *header_text = (CTB_TRACEBACK_HEADER && CTB_TRACEBACK_HEADER[0])
                                      ? CTB_TRACEBACK_HEADER
                                      : "Traceback";

        if (num_errors > 1)
        {
            fprintf(stream, "%s(#%02d)%s ", theme.error, e + 1, theme.reset);
        }

        fprintf(
            stream,
            "%s%s%s %s(most recent call last):%s\n",
            theme.error_bold,
            header_text,
            theme.reset,
            theme.error,
            theme.reset
        );

        /* Print Stack Frames */
        for (int i = 0; i < num_frames_to_print; i++)
        {
            print_frame(stream, i, &snapshot->call_stack_frames[i], &theme);
        }

        if (stack_frames_exceed_max)
        {
            fprintf(
                stream,
                "\n      %s[... Skipped %d frames ...]%s\n\n",
                theme.tb_text,
                num_frames - CTB_MAX_CALL_STACK_DEPTH,
                theme.reset
            );
        }

        print_frame(stream, num_frames, &snapshot->error_frame, &theme);

        fprintf(
            stream,
            "%s%s:%s %s%s%s\n",
            theme.error_bold,
            error_to_string(snapshot->error),
            theme.reset,
            theme.error,
            snapshot->error_message,
            theme.reset
        );

        if (e < (num_errors_to_print - 1))
        {
            fprintf(
                stream,
                "\n%sDuring handling of the above exception, another exception "
                "occurred:%s\n\n",
                theme.tb_another_exception,
                theme.reset
            );
        }
    }

    if (num_errors > CTB_MAX_NUM_ERROR)
    {
        fprintf(
            stream,
            "\n%s[... Truncated %d errors ...]%s\n",
            theme.error_bold,
            num_errors - CTB_MAX_NUM_ERROR,
            theme.reset
        );
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
 * \param[in] theme The theme to use for coloring.
 * \param[in] row_idx The current row index.
 * \param[in] label The label text.
 * \param[in] value The value text.
 */
static void print_compilation_info_row(
    FILE *stream,
    const Theme *theme,
    const int row_idx,
    const char *restrict label,
    const char *restrict value
)
{
    const int logo_height = sizeof(LOGO_LINES) / sizeof(LOGO_LINES[0]);
    const int left_padding = 2;
    const int gutter = 4;
    const int logo_width = strlen(LOGO_LINES[0]);

    if (row_idx < logo_height)
    {
        fprintf(
            stream,
            "%*s%s%s%s%*s",
            left_padding,
            "",
            theme->theme_bold,
            LOGO_LINES[row_idx],
            theme->reset,
            gutter,
            ""
        );
    }
    else
    {
        fprintf(stream, "%*s", left_padding + logo_width + gutter, "");
    }

    if (label)
    {
        if (value)
        {
            fprintf(stream, "%s%s%s%s", theme->theme_bold, label, theme->reset, value);
        }
        else
        {
            fprintf(stream, "%s%s%s", theme->theme_bold, label, theme->reset);
        }
    }

    fputs("\n", stream);
}

void ctb_print_compilation_info(void)
{
    FILE *const stream = stdout;
    const bool use_color = should_use_color(stream);
    const Theme theme = get_theme(use_color);
    const char *dash = get_dash(stream);
    const int logo_height = sizeof(LOGO_LINES) / sizeof(LOGO_LINES[0]);

    /* Determine OS String */
    const char *os_str = "Unknown";
#ifdef _WIN32
    os_str = "Windows";
#elif __APPLE__
    os_str = "MacOS";
#elif __linux__
    os_str = "Linux";
#endif

    /* Determine Compiler String */
    char compiler_str[64];
#ifdef _MSC_VER
    snprintf(compiler_str, sizeof(compiler_str), "MSVC (version: %d)", _MSC_VER);
#elif defined(__clang__)
    snprintf(
        compiler_str, sizeof(compiler_str), "Clang (version: %d)", __clang_major__
    );
#elif defined(__GNUC__)
    snprintf(compiler_str, sizeof(compiler_str), "GCC (version: %d)", __GNUC__);
#else
    snprintf(compiler_str, sizeof(compiler_str), "Unknown");
#endif

    /* Print Header */
    print_hrule_with_header(
        stream, use_color, CTB_THEME_COLOR, "C Traceback Compilation Info"
    );

    /* Prepare Config Values for printing */
    char buf_ver[32];
    char buf_date[64];
    char buf_stack[16];
    char buf_msg[16];
    char buf_err[16];
    char buf_term[16];
    char buf_file[16];
    char buf_hmax[16];
    char buf_hmin[16];

    snprintf(buf_ver, sizeof(buf_ver), "%s", CTB_VERSION);
    snprintf(buf_date, sizeof(buf_date), "%s %s", __DATE__, __TIME__);
    snprintf(buf_stack, sizeof(buf_stack), "%d", CTB_MAX_CALL_STACK_DEPTH);
    snprintf(buf_msg, sizeof(buf_msg), "%d", CTB_MAX_ERROR_MESSAGE_LENGTH);
    snprintf(buf_err, sizeof(buf_err), "%d", CTB_MAX_NUM_ERROR);
    snprintf(buf_term, sizeof(buf_term), "%d", CTB_DEFAULT_TERMINAL_WIDTH);
    snprintf(buf_file, sizeof(buf_file), "%d", CTB_DEFAULT_FILE_WIDTH);
    snprintf(buf_hmax, sizeof(buf_hmax), "%d", CTB_HRULE_MAX_WIDTH);
    snprintf(buf_hmin, sizeof(buf_hmin), "%d", CTB_HRULE_MIN_WIDTH);

    /* Construct Separator Line */
    char separator_line[16] = {0};
    for (int i = 0; i < 6; i++)
    {
        strcat(separator_line, dash);
    }

    /* Print Info Rows Linearly */
    int row = 0;
    print_compilation_info_row(stream, &theme, row++, "C Traceback Version: ", buf_ver);
    print_compilation_info_row(stream, &theme, row++, "Operating System: ", os_str);
    print_compilation_info_row(stream, &theme, row++, "Build Date: ", buf_date);
    print_compilation_info_row(stream, &theme, row++, "Compiler: ", compiler_str);
    print_compilation_info_row(stream, &theme, row++, "", NULL); /* Empty Row */
    print_compilation_info_row(stream, &theme, row++, "Config", NULL);
    print_compilation_info_row(stream, &theme, row++, separator_line, NULL);
    print_compilation_info_row(
        stream, &theme, row++, "Max Call Stack Depth: ", buf_stack
    );
    print_compilation_info_row(
        stream, &theme, row++, "Max Error Message Length: ", buf_msg
    );
    print_compilation_info_row(
        stream, &theme, row++, "Max Number of Errors: ", buf_err
    );
    print_compilation_info_row(
        stream, &theme, row++, "Default Terminal Width: ", buf_term
    );
    print_compilation_info_row(stream, &theme, row++, "Default File Width: ", buf_file);
    print_compilation_info_row(
        stream, &theme, row++, "Horizontal Rule Max Width: ", buf_hmax
    );
    print_compilation_info_row(
        stream, &theme, row++, "Horizontal Rule Min Width: ", buf_hmin
    );

    /* Fill remaining logo space */
    while (row < logo_height)
    {
        print_compilation_info_row(stream, &theme, row, NULL, NULL);
        row++;
    }

    /* Sample inline logging */
    fputs("\n", stream);
    fprintf(stream, "%sInline logging (example)%s\n", theme.theme_bold, theme.reset);
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
        75, "example/libs/utils.c", "recursion", "<error thrown here>"
    };

    fputs("\n", stream);
    fprintf(stream, "%sTraceback (example)%s\n", theme.theme_bold, theme.reset);
    for (int d = 0; d < 19; d++)
    {
        fputs(dash, stream);
    }
    fputs("\n", stream);

    const char *header_text = (CTB_TRACEBACK_HEADER && CTB_TRACEBACK_HEADER[0])
                                  ? CTB_TRACEBACK_HEADER
                                  : "Traceback";
    fprintf(
        stream,
        "%s%s%s %s(most recent call last):%s\n",
        theme.error_bold,
        header_text,
        theme.reset,
        theme.error,
        theme.reset
    );

    for (int i = 0; i < num_examples; i++)
    {
        print_frame(stream, i, &example_frames[i], &theme);
    }

    fprintf(
        stream,
        "\n      %s[... Skipped %d frames ...]%s\n\n",
        theme.tb_text,
        123,
        theme.reset
    );

    print_frame(stream, 127, &error_frame, &theme);
    fprintf(
        stream,
        "%s%s:%s %s%s%s\n",
        theme.error_bold,
        error_to_string(CTB_EXCEPTION),
        theme.reset,
        theme.error,
        "Something went wrong!",
        theme.reset
    );

    print_hrule_with_header(stream, use_color, CTB_THEME_COLOR, "END");
    fflush(stream);
}
