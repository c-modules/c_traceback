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
        ctb_raise_error(CTB_MEMORY_ERROR, "Failed to allocate memory");
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
    TRACE(division_vec(vec, 0)); // Throw error
}

static void division_vec(double *vec, double denominator)
{
    if (denominator == 0)
    {
        ctb_raise_error(
            CTB_VALUE_ERROR, "Denominator must be nonzero! Received: %lf", denominator
        );
        return;
    }

    for (int i = 0; i < N; i++)
    {
        vec[i] /= denominator;
    }
}
