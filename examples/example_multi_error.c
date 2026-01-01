#include "c_traceback.h"
#include <stdio.h>

#define FILE_PATH1 "../test1.txt"
#define FILE_PATH2 "../test2.txt"

void open_file(const char *file_name)
{
    FILE *file = fopen(file_name, "r");
    if (!file)
    {
        RAISE_ERROR_FMT(
            CTB_FILE_NOT_FOUND_ERROR, "Failed to open file: \"%s\"", file_name
        );
        return;
    }
    /* Do something */
    fclose(file);
}

void do_something_risky()
{
    int x = 0;
    if (x == 0)
    {
        RAISE_ERROR(CTB_RUNTIME_ERROR, "Division by zero attempted");
        return;
    }
    int y = 10 / x;
    (void)y;
}

int main(void)
{
    TRACE(open_file(FILE_PATH1));
    TRACE(open_file(FILE_PATH2));
    TRY_GOTO(do_something_risky(), error);
    /* Do something */
    return 0;

error:
    ctb_dump_traceback();
    return 1;
}
