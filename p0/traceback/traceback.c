/** @file traceback.c
 *  @brief The traceback function.
 *
 *  This file contains the traceback function for the traceback library.
 *
 *  @author zgw
 *  @bug Unimplemented
 */

#include <string.h>

#include "assembly_util.h"
#include "traceback_internal.h"
#include "util.h"

/** @brief Returns the index in functions if addr is a function's start address.
 *         Returns -1 otherwise.
 *  @param addr The address to check.
 *  @return Index to the function with start address addr in functions.
 */
// TODO(zgw): binary search
int addr_is_function(void *addr) {
  int i;
  for (i = 0; i < FUNCTS_MAX_NUM; ++i) {
    if (strlen(functions[i].name) == 0) {
      return -1;
    }
    if (functions[i].addr == addr) {
      return i;
    }
  }
  return -1;
}

/** @brief Given the return address, find the fuction it belongs to.
 *  @param ret_addr Return address.
 *  @return Index to the function in functions or -1 if not found.
 */
int find_function_by_ret_addr(void* ret_addr) {
  int i;
  for (i = 0; i < MAX_FUNCTION_SIZE_BYTES; ++i) {
    int index = addr_is_function(ret_addr--);
    if (index != -1) {
      return index;
    }
  }
  return -1;
}

/** @brief Output stack trace to fp.
 *  @param fp The file where the output should go.
 *  @return Void.
 */
void traceback(FILE *fp) {
  void* ebp = frame_addr();
  int index;
  do {
    void* addr = caller_ret_addr(ebp);
    ebp = caller_frame_addr(ebp);
    index = find_function_by_ret_addr(addr);
    if (index != -1) {
      if (strcmp("__libc_start_main", functions[index].name) == 0) {
        fputs("Function __libc_start_main\n", fp);
        return;
      }
      char function_string[BUFFER_SIZE];
      get_function_string(&functions[index], ebp, function_string);
      fputs(function_string, fp);
    } else {
			fprintf(fp, "Function %p(...), in\n", addr);
		}
  } while (1);
}
