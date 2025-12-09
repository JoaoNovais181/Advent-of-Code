#include<stdio.h>
#include<string.h>
#include<assert.h>

#define MAX_BUFFER 128


int biggestInRange(char *seq, int from, int to, int *biggest) {
  assert(seq && to > from);

  // printf("%s %d %d\n", seq, from, to);

  *biggest = seq[from] - '0';
  int idx = from;

  for (int i=from+1 ; i<to ; i++) {
    int val = seq[i] - '0';
    if (val > *biggest) { 
      *biggest = val;
      idx = i;
    }
  }

  return idx;
}

void challenge1() {
  FILE *fp = fopen("input.txt", "r");

  if (!fp) {
    perror("Error: couldn't open input file: ");
    return;
  }

  char buffer[MAX_BUFFER];

  long totalJoltage = 0;

  while(fgets(buffer, MAX_BUFFER - 1, fp)) {
    size_t len = strlen(buffer);   

    if (buffer[len - 1] == '\n') buffer[len - 1] = 0;
    len--;

    int joltage = 0;
    int numDigits = 2;
    int from = 0, to;


    for (int i=0 ; i<2 ; i++) {
      to = len - (numDigits - 1 - i);
      int biggest;
      from = biggestInRange(buffer, from, to, &biggest) + 1;

      joltage = joltage * 10 + biggest;
    }

    totalJoltage += joltage;
  }

  printf("The total joltage is: %ld\n", totalJoltage);
} 

void challenge2() {
  FILE *fp = fopen("input.txt", "r");

  if (!fp) {
    perror("Error: couldn't open input file: ");
    return;
  }

  char buffer[MAX_BUFFER];

  long totalJoltage = 0;

  while(fgets(buffer, MAX_BUFFER - 1, fp)) {
    size_t len = strlen(buffer);   

    if (buffer[len - 1] == '\n') buffer[len - 1] = 0;
    len--;

    long joltage = 0;
    int numDigits = 12;
    int from = 0, to;


    for (int i=0 ; i<numDigits ; i++) {
      to = len - (numDigits - 1 - i);
      int biggest;
      from = biggestInRange(buffer, from, to, &biggest) + 1;

      joltage = joltage * 10 + biggest;
    }

    totalJoltage += joltage;
  }

  printf("The total joltage is: %ld\n", totalJoltage);
} 


int main() {
  challenge1();
  challenge2();

  return 0;
}
