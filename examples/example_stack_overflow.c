#include <stdio.h>

#include "c_traceback.h"

void stack_overflow(int depth)
{
    char buffer[10240]; // Allocate some stack space
    printf("Recursion depth: %d\n", depth);
    stack_overflow(depth + 1);
}

int main(void)
{
    TRACE(ctb_install_default_signal_handler());
    THROW(CTB_BUFFER_ERROR, "Hello! This is a test error before stack overflow.");

    // Stack overflow
    TRACE(stack_overflow(1));

    return 0;
}
