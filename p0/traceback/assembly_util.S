/** @file assembly_util.c
 *  @brief Implementation of the helper functions in assembly.
 *
 *  @author zgw
 */

/* Define the functions so that they can be called from other files. */
.global frame_addr
.global caller_ret_addr
.global caller_frame_addr


frame_addr:
            movl    %ebp,%eax
            ret

caller_ret_addr:
            movl    4(%esp),%eax        /* Get argument callee's %ebp */
            movl    4(%eax),%eax        /* Store return address in return value */
            ret

caller_frame_addr:
            movl    4(%esp),%eax        /* Get argument callee's %ebp */
            movl    (%eax),%eax         /* Store old %ebp into return value */
            ret


