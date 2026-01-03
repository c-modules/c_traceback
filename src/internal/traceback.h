/**
 * \file context.h
 * \brief Definitions of context and related data structures, and related function
 * headers for C Traceback library.
 *
 * \author Ching-Yin Ng
 */

#ifndef C_TRACEBACK_INTERNAL_TRACEBACK_H
#define C_TRACEBACK_INTERNAL_TRACEBACK_H

#include "c_traceback.h"

/**
 * \brief Dump the traceback to stderr on signal error.
 *
 * \param[in] ctb_error Error type.
 */
void ctb_dump_traceback_signal(const CTB_Error ctb_error);

#endif /* C_TRACEBACK_INTERNAL_TRACEBACK_H */
