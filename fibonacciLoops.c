// program to print the fibonacci series using loops
#include <stdio.h>

void printFib(int n) {
  // if the number of terms is smaller than 1
  if (n < 1) {
    printf("Invalid number of terms.\n");
    return;
  }

  // first two terms
  int prev1 = 1;
  int prev2 = 0;

  // for loop to print n terms of series
  for (int i = 1; i <= n; ++i) {

    // print current term and update previous
    if (i > 2) {
      int curr = prev1 + prev2;
      prev2 = prev1;
      prev1 = curr;
      printf("%d ", curr);
    }
    else if (i == 1)
      printf("%d ", prev2);
    else if (i == 2)
      printf("%d ", prev1);
  }
}

int main() {
  int n = 9;

  // printing first n terms
  printFib(n);
  return 0;
}
