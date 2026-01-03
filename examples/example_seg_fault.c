#include <stdio.h>

#include "c_traceback.h"

void some_function(void)
{
    int *ptr = NULL;
    THROW(CTB_BLOCKING_IO_ERROR, "Hello! This is another test error before segfault.");
    printf("value: %d\n", *ptr); // This will cause a segmentation fault
}

int main(void)
{
    ctb_install_default_signal_handler();
    THROW(CTB_BUFFER_ERROR, "Hello! This is a test error before segfault.");

    TRACE(some_function());

    return 0;
}
