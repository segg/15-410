/** @file util.c
 *  @brief Implementation of utility functions.
 *
 *  @author zgw
 */

#include "util.h"

#include <ctype.h>
#include <string.h>
#include <sys/mman.h>

#define STRING_MAX_LENGTH 25
#define STRING_ARRAY_MEMBER_MAX_NUM 3

/** @brief Returns the name of the type as a string. It is suffixed with a white
 *         space if needed for formatting purpose.
 *  @param type The type.
 *  @return String representing the type.
 */
char* type_name(int type) {
  switch (type) {
    case TYPE_CHAR:
      return "char ";
    case TYPE_INT:
      return "int ";
    case TYPE_FLOAT:
      return "float ";
    case TYPE_DOUBLE:
      return "double ";
    case TYPE_STRING:
      return "char *";
    case TYPE_STRING_ARRAY:
      return "char **";
    case TYPE_VOIDSTAR:
      return "void *";
    default:
      return "UNKNOWN";
  }
}

/** @brief Formats string and stores in buf.
 *  @param addr Pointer to the string.
 *  @param buf Buffer to store the result.
 *  @return Void.
 */
void format_string(char* addr, char* buf) {
  memset(buf, '\0', BUFFER_SIZE);
  /* Check if memory in [addr, addr + 25] is valid */
  int ret = mprotect(
      (void*)((int)addr & 0xfffff000),
      ((int)addr & 0x00000fff) + STRING_MAX_LENGTH + 1,
      PROT_WRITE|PROT_READ|PROT_EXEC);
  if (ret != 0) {
    snprintf(buf, BUFFER_SIZE, "%p", addr);
    return;
  }

  int len = strlen(addr);
  int i;
  for (i = 0; i < len && i < STRING_MAX_LENGTH; ++i) {
    if (!isprint(*(addr + i))) {
      snprintf(buf, BUFFER_SIZE, "%p", addr);
      return;
    }
  }
  if (len > STRING_MAX_LENGTH) {
    strcat(buf, "\"");
    strncpy(buf + 1, addr, STRING_MAX_LENGTH);
    buf[STRING_MAX_LENGTH + 1] = '\0';
    strcat(buf, "...\"");
  } else {
    snprintf(buf, BUFFER_SIZE, "\"%s\"", addr);
  }
}

/** @brief Formats string array and stores in buf.
 *  @param addr Pointer to the string array.
 *  @param buf Buffer to store the result.
 *  @return Void.
 */
void format_string_array(char** addr, char* buf) {
  strcat(buf, "{");
  int i;
  for (i = 0; i < STRING_ARRAY_MEMBER_MAX_NUM; ++i) {
    if (*addr == NULL) {
      break;
    }
    char string_buf[BUFFER_SIZE];
    format_string(*addr, string_buf);
    strcat(buf, string_buf);
    ++addr;
    if (*addr != NULL && i < STRING_ARRAY_MEMBER_MAX_NUM - 1) {
      strcat(buf, ", ");
    }
  }
  if (*addr != NULL) {
    strcat(buf, ", ...");
  }
  strcat(buf, "}");
}

/** @brief Formats argument value and stores in buf.
 *  @param type Type of the value.
 *  @param addr Pointer to the value.
 *  @param buf Buffer to store the result.
 *  @return Void.
 */
void get_value_string(int type, void* addr, char* buf) {
  memset(buf, '\0', BUFFER_SIZE);
  switch (type) {
    case TYPE_CHAR:
      if (isprint(*((char*)addr))) {
        snprintf(buf, BUFFER_SIZE, "'%c'", *((char*)addr));
      } else {
        snprintf(buf, BUFFER_SIZE, "'\\%d'", *((char*)addr));
      }
      return;
    case TYPE_INT:
      snprintf(buf, BUFFER_SIZE, "%d", *((int*)addr));
      return;
    case TYPE_FLOAT:
      snprintf(buf, BUFFER_SIZE, "%f", *((float*)addr));
      return;
    case TYPE_DOUBLE:
      snprintf(buf, BUFFER_SIZE, "%f", *((double*)addr));
      return;
    case TYPE_STRING:
      format_string((char*)(*(int*)addr), buf);
      return;
    case TYPE_STRING_ARRAY:
      format_string_array((char**)(*(int*)addr), buf);
      return;
    case TYPE_VOIDSTAR:
      snprintf(buf, BUFFER_SIZE, "0v%x", (unsigned int)addr);
      return;
    default:
      snprintf(buf, BUFFER_SIZE, "%p", addr);
  }
}

/** @brief Formats argument and stores in buf.
 *  @param arg The argument to format.
 *  @param ebp Stack frame start address.
 *  @param buf Buffer to store the result.
 *  @return Void.
 */
void get_arg_string(const argsym_t *arg, void* ebp, char* buf) {
  memset(buf, '\0', BUFFER_SIZE);
  snprintf(buf, BUFFER_SIZE, "%s%s=", type_name(arg->type), arg->name);
  char value_string[BUFFER_SIZE];
  get_value_string(arg->type, arg->offset + ebp, value_string);
  strcat(buf, value_string);
}


void get_function_string(const functsym_t* function, void* ebp, char* buf) {
  memset(buf, '\0', BUFFER_SIZE);
  snprintf(buf, BUFFER_SIZE, "Function %s(", function->name);
  /* Print argument list */
  int i;
  for (i = 0; i < ARGS_MAX_NUM; ++i) {
    const argsym_t *arg = &function->args[i];
    if (strlen(arg->name) == 0) {
      break;
    }
    if (i > 0) {
      strcat(buf, ", ");
    }
    char arg_string[BUFFER_SIZE];
    get_arg_string(arg, ebp, arg_string);
    strcat(buf, arg_string);
  }
  if (i == 0) {
    strcat(buf, "void");
  }
  strcat(buf, "), in\n");
}
