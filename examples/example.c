#include <stdio.h>

#include "c_traceback.h"

void nested_test_warning();

int main(void)
{
    // Test make context
    CTB_Context *ctb_context = ctb_make_context();
    if (!ctb_context)
    {
        printf("Error: unable to make ctb_context");
        goto error;
    }

    // Test raise warning
    nested_test_warning();

    return 0;

error:
    return 1;
}

void nested_test_warning()
{
    CTB_PRINT_WARNING(
        CTB_DEPRECATION_WARNING,
        "This function is deprecated in version 0.0.1. Use hello_world() instead."
    );
}