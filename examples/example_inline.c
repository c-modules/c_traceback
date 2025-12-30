/**
 * \file example_inline.c
 *
 * \brief Example usage of inline logging functions from c_traceback library
 */

#include <stdio.h>

#include "c_traceback.h"

void inline_error(int i);
void inline_error_level2(int i);
void inline_warning(int i);
void inline_warning_level2(int i);
void inline_message(int i);
void inline_message_level2(int i);

int main(void)
{
    inline_error(1);
    inline_warning(3);
    inline_message(5);

    return 0;
}

void inline_error(int i)
{
    CTB_LOG_ERROR_INLINE(
        CTB_ARITHMETIC_ERROR,
        "(Test %d) This should be inline error level 1 with arithmetic error",
        i
    );
    inline_error_level2(i + 1);
}

void inline_error_level2(int i)
{
    CTB_LOG_ERROR_INLINE(
        CTB_BUFFER_ERROR,
        "(Test %d) This should be inline error level 2 with buffer error",
        i
    );
}

void inline_warning(int i)
{
    CTB_LOG_WARNING_INLINE(
        CTB_DEPRECATION_WARNING,
        "(Test %d) This should be inline warning level 1 with deprecation warning",
        i
    );
    inline_warning_level2(i + 1);
}

void inline_warning_level2(int i)
{
    CTB_LOG_WARNING_INLINE(
        CTB_USER_WARNING,
        "(Test %d) This should be inline warning level 2 with user warning",
        i
    );
}

void inline_message(int i)
{
    CTB_LOG_MESSAGE_INLINE("(Test %d) This should be inline message level 1", i);
    inline_message_level2(i + 1);
}

void inline_message_level2(int i)
{
    CTB_LOG_MESSAGE_INLINE("(Test %d) This should be inline message level 2", i);
}
