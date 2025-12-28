#include <stdio.h>

#include "c_traceback.h"

void inline_error(int *i);
void inline_error_level2(int *i);
void inline_warning(int *i);
void inline_warning_level2(int *i);
void inline_message(int *i);
void inline_message_level2(int *i);
void function_a();
void function_b();
void recursion(int i);
void nested_function_to_raise_error(int i);

int main(void)
{
    // Inline functions
    int i = 0;
    inline_error(&i);
    inline_warning(&i);
    inline_message(&i);
    CTB_CHECK_GOTO(function_a(), error);
    printf("This shouldn't be printed");

error:
    ctb_dump_traceback();
    return 0;
}

void inline_error(int *i)
{
    (*i)++;
    CTB_LOG_ERROR_INLINE(
        CTB_ARITHMETIC_ERROR,
        "(Test %d) This should be inline error level 1 with arithmetic error",
        *i
    );
    inline_error_level2(i);
}

void inline_error_level2(int *i)
{
    (*i)++;
    CTB_LOG_ERROR_INLINE(
        CTB_BUFFER_ERROR,
        "(Test %d) This should be inline error level 2 with buffer error",
        *i
    );
}

void inline_warning(int *i)
{
    (*i)++;
    CTB_LOG_WARNING_INLINE(
        CTB_DEPRECATION_WARNING,
        "(Test %d) This should be inline warning level 1 with deprecation warning",
        *i
    );
    inline_warning_level2(i);
}

void inline_warning_level2(int *i)
{
    (*i)++;
    CTB_LOG_WARNING_INLINE(
        CTB_USER_WARNING,
        "(Test %d) This should be inline warning level 2 with user warning",
        *i
    );
}

void inline_message(int *i)
{
    (*i)++;
    CTB_LOG_MESSAGE_INLINE("(Test %d) This should be inline message level 1", *i);
    inline_message_level2(i);
}

void inline_message_level2(int *i)
{
    (*i)++;
    CTB_LOG_MESSAGE_INLINE("(Test %d) This should be inline message level 2", *i);
}

void function_a()
{
    CTB_WRAP(function_b());
}

void function_b()
{
    int i = 0;
    CTB_WRAP(recursion(i));
    ctb_raise_error(CTB_CHILD_PROCESS_ERROR, "Hello :)");
}

void recursion(int i)
{
    if (i >= 125)
    {
        return;
    }

    if (i == 2)
    {
        CTB_WRAP(nested_function_to_raise_error(i));
    }

    CTB_WRAP(recursion(i + 1));
}

void nested_function_to_raise_error(int i)
{
    ctb_raise_error(CTB_VALUE_ERROR, "Test: current recursion depth: %d", i);
}