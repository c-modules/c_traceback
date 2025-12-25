#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "c_traceback.h"
#include "internal/utils.h"

#if defined(_WIN32)
#include <io.h>
#include <windows.h>
#define ISATTY _isatty
#define FILENO _fileno
#else
#include <unistd.h>
#define ISATTY isatty
#define FILENO fileno
#endif

bool should_use_color(FILE *stream)
{
    // NO_COLOR set
    // Don’t output ANSI color escape codes, see no-color.org
    const char *no_color = getenv("NO_COLOR");
    if (no_color && no_color[0] != '\0')
    {
        return false;
    }

    // CLICOLOR_FORCE set, but NO_COLOR unset
    // ANSI colors should be enabled no matter what
    const char *force_color = getenv("CLICOLOR_FORCE");
    if (force_color && strcmp(force_color, "0") != 0)
    {
        return true;
    }

    // Check if the stream is a terminal
    int fd = FILENO(stream);
    if (!ISATTY(fd))
    {
        return false;
    }

    // Check TERM variable for "dumb" terminals
    const char *term = getenv("TERM");
    if (term && strcmp(term, "dumb") == 0)
    {
        return false;
    }

// Windows Specific: Enable Virtual Terminal Processing (ANSI support)
#if defined(_WIN32)
    HANDLE hOut = (HANDLE)_get_osfhandle(fd);
    if (hOut == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode))
    {
        return false;
    }

// ENABLE_VIRTUAL_TERMINAL_PROCESSING might not be defined in older SDKs
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

    if (!(dwMode & ENABLE_VIRTUAL_TERMINAL_PROCESSING))
    {
        if (!SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING))
        {
            return false; // Failed to enable ANSI support on this Windows console
        }
    }
#endif

    return true;
}

const char *error_code_to_string(int code)
{
    switch (code)
    {
        case CTB_UNKNOWN_ERROR:
            return "Unknown Error";
        case CTB_SUCCESS:
            return "Success";

            /* ------------------------------------------------------------
             *  Base Exception & Direct Subclasses (1 - 19)
             * ------------------------------------------------------------ */
        case CTB_BASE_EXCEPTION:
            return "BaseException";
        case CTB_GENERATOR_EXIT:
            return "GeneratorExit";
        case CTB_KEYBOARD_INTERRUPT:
            return "KeyboardInterrupt";
        case CTB_SYSTEM_EXIT:
            return "SystemExit";
        case CTB_EXCEPTION:
            return "Exception";

            /* ------------------------------------------------------------
             *  Arithmetic Errors (20 - 29)
             * ------------------------------------------------------------ */
        case CTB_ARITHMETIC_ERROR:
            return "ArithmeticError";
        case CTB_FLOATING_POINT_ERROR:
            return "FloatingPointError";
        case CTB_OVERFLOW_ERROR:
            return "OverflowError";
        case CTB_ZERO_DIVISION_ERROR:
            return "ZeroDivisionError";

            /* ------------------------------------------------------------
             *  Standard Logic & Lookup Errors (30 - 59)
             * ------------------------------------------------------------ */
        case CTB_ASSERTION_ERROR:
            return "AssertionError";
        case CTB_ATTRIBUTE_ERROR:
            return "AttributeError";
        case CTB_BUFFER_ERROR:
            return "BufferError";
        case CTB_EOF_ERROR:
            return "EOFError";

        case CTB_IMPORT_ERROR:
            return "ImportError";
        case CTB_MODULE_NOT_FOUND_ERROR:
            return "ModuleNotFoundError";

        case CTB_LOOKUP_ERROR:
            return "LookupError";
        case CTB_INDEX_ERROR:
            return "IndexError";
        case CTB_KEY_ERROR:
            return "KeyError";

        case CTB_MEMORY_ERROR:
            return "MemoryError";

        case CTB_NAME_ERROR:
            return "NameError";
        case CTB_UNBOUND_LOCAL_ERROR:
            return "UnboundLocalError";

        case CTB_REFERENCE_ERROR:
            return "ReferenceError";

            /* ------------------------------------------------------------
             *  OS Errors (60 - 99)
             * ------------------------------------------------------------ */
        case CTB_OS_ERROR:
            return "OSError";
        case CTB_BLOCKING_IO_ERROR:
            return "BlockingIOError";
        case CTB_CHILD_PROCESS_ERROR:
            return "ChildProcessError";
        case CTB_CONNECTION_ERROR:
            return "ConnectionError";
        case CTB_BROKEN_PIPE_ERROR:
            return "BrokenPipeError";
        case CTB_CONNECTION_ABORTED_ERROR:
            return "ConnectionAbortedError";
        case CTB_CONNECTION_REFUSED_ERROR:
            return "ConnectionRefusedError";
        case CTB_CONNECTION_RESET_ERROR:
            return "ConnectionResetError";
        case CTB_FILE_EXISTS_ERROR:
            return "FileExistsError";
        case CTB_FILE_NOT_FOUND_ERROR:
            return "FileNotFoundError";
        case CTB_INTERRUPTED_ERROR:
            return "InterruptedError";
        case CTB_IS_A_DIRECTORY_ERROR:
            return "IsADirectoryError";
        case CTB_NOT_A_DIRECTORY_ERROR:
            return "NotADirectoryError";
        case CTB_PERMISSION_ERROR:
            return "PermissionError";
        case CTB_PROCESS_LOOKUP_ERROR:
            return "ProcessLookupError";
        case CTB_TIMEOUT_ERROR:
            return "TimeoutError";
            /* ------------------------------------------------------------
             *  Runtime & Syntax Errors (100 - 129)
             * ------------------------------------------------------------ */
        case CTB_RUNTIME_ERROR:
            return "RuntimeError";
        case CTB_NOT_IMPLEMENTED_ERROR:
            return "NotImplementedError";
        case CTB_PYTHON_FINALIZATION_ERROR:
            return "PythonFinalizationError";
        case CTB_RECURSION_ERROR:
            return "RecursionError";

        case CTB_STOP_ASYNC_ITERATION:
            return "StopAsyncIteration";
        case CTB_STOP_ITERATION:
            return "StopIteration";

        case CTB_SYNTAX_ERROR:
            return "SyntaxError";
        case CTB_INDENTATION_ERROR:
            return "IndentationError";
        case CTB_TAB_ERROR:
            return "TabError";

        case CTB_SYSTEM_ERROR:
            return "SystemError";
            /* ------------------------------------------------------------
             *  Type & Value Errors (130 - 149)
             * ------------------------------------------------------------ */
        case CTB_TYPE_ERROR:
            return "TypeError";

        case CTB_VALUE_ERROR:
            return "ValueError";
        case CTB_UNICODE_ERROR:
            return "UnicodeError";
        case CTB_UNICODE_DECODE_ERROR:
            return "UnicodeDecodeError";
        case CTB_UNICODE_ENCODE_ERROR:
            return "UnicodeEncodeError";
        case CTB_UNICODE_TRANSLATE_ERROR:
            return "UnicodeTranslateError";
            /* ------------------------------------------------------------
             *  Warnings (150+)
             * ------------------------------------------------------------ */
        case CTB_WARNING:
            return "Warning";
        case CTB_BYTES_WARNING:
            return "BytesWarning";
        case CTB_DEPRECATION_WARNING:
            return "DeprecationWarning";
        case CTB_ENCODING_WARNING:
            return "EncodingWarning";
        case CTB_FUTURE_WARNING:
            return "FutureWarning";
        case CTB_IMPORT_WARNING:
            return "ImportWarning";
        case CTB_PENDING_DEPRECATION_WARNING:
            return "PendingDeprecationWarning";
        case CTB_RESOURCE_WARNING:
            return "ResourceWarning";
        case CTB_RUNTIME_WARNING:
            return "RuntimeWarning";
        case CTB_SYNTAX_WARNING:
            return "SyntaxWarning";
        case CTB_UNICODE_WARNING:
            return "UnicodeWarning";
        case CTB_USER_WARNING:
            return "UserWarning";
        default:
            return "Unknown Error";
    }
}