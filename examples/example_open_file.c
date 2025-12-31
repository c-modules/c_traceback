#include <stdio.h>

#include "c_traceback.h"

#define FILE_PATH "../test.txt"

void open_file(const char *file_name)
{
    FILE *file = fopen(file_name, "r");
    if (!file)
    {
        RAISE_ERROR(CTB_FILE_NOT_FOUND_ERROR, "Failed to open file: \"%s\"", file_name);
        return;
    }
    /* Do something */
    fclose(file);
}

int main(void)
{
    TRY_GOTO(open_file(FILE_PATH), error);
    /* Do something */

    return 0;

error:
    ctb_dump_traceback();
    return 1;
}