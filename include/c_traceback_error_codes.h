/**
 * \file c_traceback_error_codes.h
 * \brief Error codes based on Python 3.14.2 Exceptions
 * \details This header defines error codes for the c_traceback library,
 *          based on the exception hierarchy of Python 3.14.2. For more
 *          details, see https://docs.python.org/3/library/exceptions.html
 *
 * \author Ching-Yin Ng
 */

#ifndef C_TRACEBACK_CODES_H
#define C_TRACEBACK_CODES_H

/* Python 3.14.2 Exception Hierarchy
    BaseException
    ├── BaseExceptionGroup
    ├── GeneratorExit
    ├── KeyboardInterrupt
    ├── SystemExit
    └── Exception
        ├── ArithmeticError
        │    ├── FloatingPointError
        │    ├── OverflowError
        │    └── ZeroDivisionError
        ├── AssertionError
        ├── AttributeError
        ├── BufferError
        ├── EOFError
        ├── ExceptionGroup [BaseExceptionGroup]
        ├── ImportError
        │    └── ModuleNotFoundError
        ├── LookupError
        │    ├── IndexError
        │    └── KeyError
        ├── MemoryError
        ├── NameError
        │    └── UnboundLocalError
        ├── OSError
        │    ├── BlockingIOError
        │    ├── ChildProcessError
        │    ├── ConnectionError
        │    │    ├── BrokenPipeError
        │    │    ├── ConnectionAbortedError
        │    │    ├── ConnectionRefusedError
        │    │    └── ConnectionResetError
        │    ├── FileExistsError
        │    ├── FileNotFoundError
        │    ├── InterruptedError
        │    ├── IsADirectoryError
        │    ├── NotADirectoryError
        │    ├── PermissionError
        │    ├── ProcessLookupError
        │    └── TimeoutError
        ├── ReferenceError
        ├── RuntimeError
        │    ├── NotImplementedError
        │    ├── PythonFinalizationError
        │    └── RecursionError
        ├── StopAsyncIteration
        ├── StopIteration
        ├── SyntaxError
        │    └── IndentationError
        │         └── TabError
        ├── SystemError
        ├── TypeError
        ├── ValueError
        │    └── UnicodeError
        │         ├── UnicodeDecodeError
        │         ├── UnicodeEncodeError
        │         └── UnicodeTranslateError
        └── Warning
            ├── BytesWarning
            ├── DeprecationWarning
            ├── EncodingWarning
            ├── FutureWarning
            ├── ImportWarning
            ├── PendingDeprecationWarning
            ├── ResourceWarning
            ├── RuntimeWarning
            ├── SyntaxWarning
            ├── UnicodeWarning
            └── UserWarning
*/

#define CTB_UNKNOWN_ERROR -1
#define CTB_SUCCESS 0

/* ------------------------------------------------------------
 *  Base Exception & Direct Subclasses (1 - 19)
 * ------------------------------------------------------------ */
#define CTB_BASE_EXCEPTION 1
#define CTB_GENERATOR_EXIT 2
#define CTB_KEYBOARD_INTERRUPT 3
#define CTB_SYSTEM_EXIT 4
#define CTB_EXCEPTION 5

/* ------------------------------------------------------------
 *  Arithmetic Errors (20 - 29)
 * ------------------------------------------------------------ */
#define CTB_ARITHMETIC_ERROR 20
#define CTB_FLOATING_POINT_ERROR 21
#define CTB_OVERFLOW_ERROR 22
#define CTB_ZERO_DIVISION_ERROR 23

/* ------------------------------------------------------------
 *  Standard Logic & Lookup Errors (30 - 59)
 * ------------------------------------------------------------ */
#define CTB_ASSERTION_ERROR 30
#define CTB_ATTRIBUTE_ERROR 31
#define CTB_BUFFER_ERROR 32
#define CTB_EOF_ERROR 33

#define CTB_IMPORT_ERROR 35
#define CTB_MODULE_NOT_FOUND_ERROR 36

#define CTB_LOOKUP_ERROR 40
#define CTB_INDEX_ERROR 41
#define CTB_KEY_ERROR 42

#define CTB_MEMORY_ERROR 45

#define CTB_NAME_ERROR 50
#define CTB_UNBOUND_LOCAL_ERROR 51

#define CTB_REFERENCE_ERROR 55

/* ------------------------------------------------------------
 *  OS Errors (60 - 99)
 * ------------------------------------------------------------ */
#define CTB_OS_ERROR 60
#define CTB_BLOCKING_IO_ERROR 61
#define CTB_CHILD_PROCESS_ERROR 62
#define CTB_CONNECTION_ERROR 63
#define CTB_BROKEN_PIPE_ERROR 64
#define CTB_CONNECTION_ABORTED_ERROR 65
#define CTB_CONNECTION_REFUSED_ERROR 66
#define CTB_CONNECTION_RESET_ERROR 67
#define CTB_FILE_EXISTS_ERROR 68
#define CTB_FILE_NOT_FOUND_ERROR 69
#define CTB_INTERRUPTED_ERROR 70
#define CTB_IS_A_DIRECTORY_ERROR 71
#define CTB_NOT_A_DIRECTORY_ERROR 72
#define CTB_PERMISSION_ERROR 73
#define CTB_PROCESS_LOOKUP_ERROR 74
#define CTB_TIMEOUT_ERROR 75

/* ------------------------------------------------------------
 *  Runtime & Syntax Errors (100 - 129)
 * ------------------------------------------------------------ */
#define CTB_RUNTIME_ERROR 100
#define CTB_NOT_IMPLEMENTED_ERROR 101
#define CTB_PYTHON_FINALIZATION_ERROR 102
#define CTB_RECURSION_ERROR 103

#define CTB_STOP_ASYNC_ITERATION 105
#define CTB_STOP_ITERATION 106

#define CTB_SYNTAX_ERROR 110
#define CTB_INDENTATION_ERROR 111
#define CTB_TAB_ERROR 112

#define CTB_SYSTEM_ERROR 115

/* ------------------------------------------------------------
 *  Type & Value Errors (130 - 149)
 * ------------------------------------------------------------ */
#define CTB_TYPE_ERROR 130

#define CTB_VALUE_ERROR 135
#define CTB_UNICODE_ERROR 136
#define CTB_UNICODE_DECODE_ERROR 137
#define CTB_UNICODE_ENCODE_ERROR 138
#define CTB_UNICODE_TRANSLATE_ERROR 139

/* ------------------------------------------------------------
 *  Warnings (150+)
 * ------------------------------------------------------------ */
#define CTB_WARNING 150
#define CTB_BYTES_WARNING 151
#define CTB_DEPRECATION_WARNING 152
#define CTB_ENCODING_WARNING 153
#define CTB_FUTURE_WARNING 154
#define CTB_IMPORT_WARNING 155
#define CTB_PENDING_DEPRECATION_WARNING 156
#define CTB_RESOURCE_WARNING 157
#define CTB_RUNTIME_WARNING 158
#define CTB_SYNTAX_WARNING 159
#define CTB_UNICODE_WARNING 160
#define CTB_USER_WARNING 161

#endif