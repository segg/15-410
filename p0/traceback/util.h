/** @file util.h
 *  @brief Some utility functions.
 *
 *  @author zgw
 */

#ifndef __UTIL_H
#define __UTIL_H

#include "traceback_internal.h"

#define BUFFER_SIZE 1024

/** @brief Stores the formatted string for the given function in buf.
 *  @param function The function to print out.
 *  @param ebp The %ebp of the function's stack frame.
 *  @param buf The buffer to store the formatted string.
 *  @return Void.
 */
void get_function_string(const functsym_t* function, void* ebp, char* buf);


#endif  /* __UTIL_H */
