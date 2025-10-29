// recursive function to print the fibonacci series
#include <stdio.h>

void fib(int n, int prev1, int prev2) {
  // base case: n gets less than 3
  if (n > 3) {
    return;
  }

  int curr = prev1 + prev2;
  printf("%d ", curr);

  return fib(n -1, prev2, curr);
}

// function that handles the first two terms and calls the recursive function
void printFib(int n) {
  // when number of terms < 1
  if (n < 1) {
    printf("Invalid number of terms\n");
  }
  // number of terms = 1
  else if (n == 1) {
    printf("%d ", 0);
  }
  // number of terms = 2
  else if (n == 2) {
    printf("%d %d", 0, 1);
  }
  // number of terms > 2
  else {
    printf("%d %d ", 0, 1);
    fib(n, 0, 1);
  }
  return;
}

int main() {
  int n = 9;

  // printing first 9 fibonacci terms
  printFib(n);

  return 0;
}
