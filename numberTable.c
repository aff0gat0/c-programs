// a program to print a table of number representations
#include <stdio.h>

int main (void)
{
  printf("Power   Decimal   Octal\n");
  printf("-----   -------   -----\n");

  for (int i = 0; i <= 12; ++i) {
    int value = 1 << i;
    printf("2^%-2d    %-7d    %o\n", i, value, value);
  }

  return 0;
}
