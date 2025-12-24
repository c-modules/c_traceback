/**
 * \file c_traceback.h
 * \brief Library header for c_traceback library
 * 
 * \author Ching-Yin Ng
 */

#ifndef C_TRACEBACK_H
#define C_TRACEBACK_H

#include "c_traceback_error_codes.h"

typedef struct CTB_Context CTB_Context;

// Maximum number of traceback frames
#define MAX_TRACEBACK_FRAMES 64

CTB_Context* ctb_make_context(void);
void ctb_free_context(CTB_Context *restrict context);


#endif