/** @file assembly_util.h
 *  @brief Provides the definitions for the helper functions, which cannot be
 *         implemented in C.
 *
 *  @author zgw
 */

#ifndef __ASSEMBLY_UTIL_H
#define __ASSEMBLY_UTIL_H

/** @brief Returns the value of frame pointer %ebp, which should be the start
 *         address of the caller's stack frame.
 *  @return The start address of the caller's stack frame.
 */
void* frame_addr();

/** @brief Returns the caller's return address given the start address of the
 *         callee's stack frame.
 *  @param frame_addr Start address of the callee's stack frame.
 *  @return Caller's return address.
 */
void* caller_ret_addr(void* frame_addr);

/** @brief Returns the caller's frame start address given the start address of
 *         the callee's stack frame.
 *  @param frame_addr Start address of the callee's stack frame.
 *  @return Caller's stack frame start address.
 */
void* caller_frame_addr(void* frame_addr);

#endif /* __ASSEMBLY_UTIL_H */
