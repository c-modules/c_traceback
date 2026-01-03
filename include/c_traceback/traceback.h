/**
 * \file traceback.c
 * \brief Header file for traceback-related macros and functions.
 *
 * \author Ching-Yin Ng
 */

#ifndef C_TRACEBACK_TRACEBACK_H
#define C_TRACEBACK_TRACEBACK_H

/**
 * \brief Log the traceback of all recorded errors to stderr.
 */
void ctb_log_error_traceback(void);

/**
 * \brief Dump the traceback of all recorded errors to stderr and clear errors.
 */
void ctb_dump_traceback(void);

/**
 * \brief Dump the traceback to stderr on signal error.
 *
 * \param[in] ctb_error Error type.
 */
void ctb_dump_traceback_signal(const CTB_Error ctb_error);

#endif /* C_TRACEBACK_TRACEBACK_H */