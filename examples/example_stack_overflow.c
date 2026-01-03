#include <stdio.h>
#include <string.h>

#include "c_traceback.h"

void stack_overflow(int depth)
{
    char buffer[10240]; // Allocate some stack space
    memset(buffer, 0, sizeof(buffer));
    printf("Recursion depth: %d\n", depth);
    stack_overflow(depth + 1);
}

int main(void)
{
    TRACE(ctb_install_signal_handler());

    // Stack overflow
    TRACE(stack_overflow(1));

    return 0;
}
