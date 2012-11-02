#include <stdio.h>
#include "traceback.h"

void bar(int x, int y, char** string_array, char** ss)
{
  int z;
  z = x + y;
  traceback(stdout);
}

void foo(char c, char* s, char* un) {
  char * sap[] = {"1234567890123456789012345", "a\3b", "12345678901234567890123456", NULL};
  char * ss[] = {"12\34", NULL, "sdf"}; 
  bar (5,17,sap, ss);
}

int main (int argc, char **argv)
{
  foo('\6', "abcdefghijklmnopqrstuvwxyz", "abc\1de");
  return 0;
}
