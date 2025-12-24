#include <stdio.h>

/* Private API */
#include "common.h"

/* Public API */
#include "c_traceback.h"

int main(void)
{
    CTB_Context* ctb_context = ctb_make_context();
    if (!ctb_context)
    {
        printf("Error: unable to make ctb_context");
        goto error;
    }
    printf("num_errors: %d", ctb_context->num_errors);
    return 0;

error:
    return 1;
}