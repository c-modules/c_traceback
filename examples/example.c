#include <stdio.h>

#include "c_traceback.h"

void inline_error();
void inline_error_level2();
void inline_warning();
void inline_warning_level2();
void inline_message();
void inline_message_level2();

int main(void)
{
    // Make context
    CTB_Context *ctb_context = ctb_make_context();
    if (!ctb_context)
    {
        fprintf(stderr, "Error: unable to make ctb_context");
        fflush(stderr);
        goto error;
    }

    // Inline functions
    printf("=============== Inline examples ===============\n");
    fflush(stdout);

    inline_error();
    inline_warning();
    inline_message();

    printf("===============================================\n");
    fflush(stdout);

    return 0;

error:
    return 1;
}

void inline_error()
{
    CTB_LOG_ERROR_INLINE(
        CTB_ARITHMETIC_ERROR,
        "This should be inline error level 1 with arithmetic error"
    );
    inline_error_level2();
}

void inline_error_level2()
{
    CTB_LOG_ERROR_INLINE(
        CTB_BUFFER_ERROR, "This should be inline error level 2 with buffer error"
    );
}

void inline_warning()
{
    CTB_LOG_WARNING_INLINE(
        CTB_DEPRECATION_WARNING,
        "This should be inline warning level 1 with deprecation warning"
    );
    inline_warning_level2();
}

void inline_warning_level2()
{
    CTB_LOG_WARNING_INLINE(
        CTB_USER_WARNING, "This should be inline warning level 2 with user warning"
    );
}

void inline_message()
{
    CTB_LOG_MESSAGE_INLINE("This should be inline message level 1");
    inline_message_level2();
}

void inline_message_level2()
{
    CTB_LOG_MESSAGE_INLINE("This should be inline message level 2");
}
