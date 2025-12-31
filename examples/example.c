#include <stdio.h>
#include <stdlib.h>

#include "c_traceback.h"

#define N 10

static void do_calculation(double *vec);
static void division_vec(double *vec, double denominator);

int main(void)
{
    double *vec = malloc(N * sizeof(double));
    if (!vec)
    {
        RAISE_ERROR(CTB_MEMORY_ERROR, "Failed to allocate memory", 123);
        goto error;
    }

    TRY_GOTO(do_calculation(vec), error);
    printf("This shouldn't be printed if there is error");

error:
    ctb_dump_traceback(); // Log traceback and reset error stack
    return 0;
}

static void do_calculation(double *vec)
{
    // Initialize array
    for (int i = 0; i < N; i++)
    {
        vec[i] = 0;
    }
    int success = TRACE(division_vec(vec, 0)); // Throw error
    if (!success)
    {
        return;
    }

    // Further calculations...
    for (int i = 0; i < N; i++)
    {
        vec[i] += 10;
    }
}

static void division_vec(double *vec, double denominator)
{
    if (denominator == 0)
    {
        RAISE_ERROR(
            CTB_VALUE_ERROR, "Denominator must be nonzero! Received: %lf", denominator
        );
        return;
    }

    for (int i = 0; i < N; i++)
    {
        vec[i] /= denominator;
    }
}
