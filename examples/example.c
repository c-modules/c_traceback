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
void function_c();
void recursion(int i);

int main(void)
{
    // Inline functions
    printf("=============== Inline examples ===============\n");
    fflush(stdout);

    int i = 0;
    inline_error(&i);
    inline_warning(&i);
    inline_message(&i);

    printf("============= Stacktrace example ==============\n");
    fflush(stdout);

    CTB_WRAP(function_a());

    printf("===============================================\n");
    fflush(stdout);

    return 0;

error:
    return 1;
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
    CTB_WRAP(function_c());
}

void function_c()
{
    int i = 0;
    CTB_WRAP(recursion(i));
}

void recursion(int i)
{
    if (i >= 125)
    {
        return;
    }

    CTB_WRAP(recursion(i + 1));
}