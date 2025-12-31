#include <stdio.h>

#include "c_traceback.h"

#define N 100

void recursion(int count)
{
    if (count >= N)
    {
        RAISE_ERROR(CTB_RUNTIME_ERROR, "Oh no, some error occurred at depth %d", count);
        return;
    }

    TRACE_VOID(recursion(count + 1));
}

int main(void)
{
    TRY_GOTO(recursion(0), error);
    printf("This shouldn't be printed if there is error");

error:
    ctb_dump_traceback(); // Log traceback and reset error stack
    return 0;
}
