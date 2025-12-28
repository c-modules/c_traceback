/**
 * \file c_traceback_errors.h
 * \brief Error codes based on Python 3.14.2 Exceptions
 * \details This header defines error codes for the c_traceback library,
 *          based on the exception hierarchy of Python 3.14.2. For more
 *          details, see https://docs.python.org/3/library/exceptions.html
 *
 * \author Ching-Yin Ng
 */

#ifndef C_TRACEBACK_ERRORS_H
#define C_TRACEBACK_ERRORS_H

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

typedef enum CTB_Error
{
    CTB_UNKNOWN_ERROR = -1,
    CTB_SUCCESS = 0,

    // Base Exception & Direct Subclasses
    CTB_BASE_EXCEPTION,
    CTB_GENERATOR_EXIT,
    CTB_KEYBOARD_INTERRUPT,
    CTB_SYSTEM_EXIT,
    CTB_EXCEPTION,

    // Arithmetic Errors
    CTB_ARITHMETIC_ERROR,
    CTB_FLOATING_POINT_ERROR,
    CTB_OVERFLOW_ERROR,
    CTB_ZERO_DIVISION_ERROR,

    // Standard Logic & Lookup Errors
    CTB_ASSERTION_ERROR,
    CTB_ATTRIBUTE_ERROR,
    CTB_BUFFER_ERROR,
    CTB_EOF_ERROR,
    CTB_IMPORT_ERROR,
    CTB_MODULE_NOT_FOUND_ERROR,
    CTB_LOOKUP_ERROR,
    CTB_INDEX_ERROR,
    CTB_KEY_ERROR,
    CTB_MEMORY_ERROR,
    CTB_NAME_ERROR,
    CTB_UNBOUND_LOCAL_ERROR,
    CTB_REFERENCE_ERROR,

    // OS Errors
    CTB_OS_ERROR,
    CTB_BLOCKING_IO_ERROR,
    CTB_CHILD_PROCESS_ERROR,
    CTB_CONNECTION_ERROR,
    CTB_BROKEN_PIPE_ERROR,
    CTB_CONNECTION_ABORTED_ERROR,
    CTB_CONNECTION_REFUSED_ERROR,
    CTB_CONNECTION_RESET_ERROR,
    CTB_FILE_EXISTS_ERROR,
    CTB_FILE_NOT_FOUND_ERROR,
    CTB_INTERRUPTED_ERROR,
    CTB_IS_A_DIRECTORY_ERROR,
    CTB_NOT_A_DIRECTORY_ERROR,
    CTB_PERMISSION_ERROR,
    CTB_PROCESS_LOOKUP_ERROR,
    CTB_TIMEOUT_ERROR,

    // Runtime & Syntax Errors
    CTB_RUNTIME_ERROR,
    CTB_NOT_IMPLEMENTED_ERROR,
    CTB_PYTHON_FINALIZATION_ERROR,
    CTB_RECURSION_ERROR,
    CTB_STOP_ASYNC_ITERATION,
    CTB_STOP_ITERATION,
    CTB_SYNTAX_ERROR,
    CTB_INDENTATION_ERROR,
    CTB_TAB_ERROR,
    CTB_SYSTEM_ERROR,

    // Type & Value Errors
    CTB_TYPE_ERROR,
    CTB_VALUE_ERROR,
    CTB_UNICODE_ERROR,
    CTB_UNICODE_DECODE_ERROR,
    CTB_UNICODE_ENCODE_ERROR,
    CTB_UNICODE_TRANSLATE_ERROR,
} CTB_Error;

typedef enum CTB_Warning
{
    CTB_UNKNOWN_WARNING = -1,
    CTB_WARNING = 0,
    CTB_BYTES_WARNING,
    CTB_DEPRECATION_WARNING,
    CTB_ENCODING_WARNING,
    CTB_FUTURE_WARNING,
    CTB_IMPORT_WARNING,
    CTB_PENDING_DEPRECATION_WARNING,
    CTB_RESOURCE_WARNING,
    CTB_RUNTIME_WARNING,
    CTB_SYNTAX_WARNING,
    CTB_UNICODE_WARNING,
    CTB_USER_WARNING
} CTB_Warning;

#endif /* C_TRACEBACK_ERRORS_H */
