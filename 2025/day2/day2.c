#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<assert.h>

#define MAX_BUFFER 1024

int charToInt(char c) {
  // printf("%c\n", c);
  assert(c >= '0' && c <= '9');

  return c - '0';
}

// 0 -> ok, != 0 -> invalid
int isInvalid(char *id, int len) {
  if (len % 2 != 0)
    return 0;

  int fst = 0, snd = 0;

  for (int i=0 ; i < len / 2 ; i++) {
    fst = fst * 10 + charToInt(id[i]);
  }
  
  for (int i=len/2 ; i < len ; i++) {
    snd = snd * 10 + charToInt(id[i]);
  }

  return fst == snd;
}

char *intToStr(long num) {
  char *str = malloc(32);

  sprintf(str, "%ld", num);

  return str;
}

void challenge1() {
  char buffer[MAX_BUFFER] = {0};

  FILE *file = fopen("input.txt", "r");

  if (!file) {
    perror("Error reading file: ");
    return;
  }

  long sum = 0;

  while(fgets(buffer, MAX_BUFFER - 1, file)) {
    int len = strlen(buffer);
    if (buffer[len - 1] == '\n') buffer[len - 1] = 0;
    
    long lower, upper;

    if (sscanf(buffer, "%ld-%ld", &lower, &upper) != 2) {
      printf("Error: invalid input");
      return;
    }

    for (long i=lower; i <= upper ; i++){
      char *str = intToStr(i);
      if (isInvalid(str, strlen(str))) {
        sum += i;
      }
      free(str);
    }
  }


  printf("The sum of invalid ID's is: %ld\n", sum);
}

int isInvalid_2(char *string) {
  int len = strlen(string);

  for (int patternLen=1 ; patternLen<=len/2 ; patternLen++) {
    if (len % patternLen != 0) continue;

    int isPattern = 1;

    for (int i=patternLen ; i < len ; i+=patternLen) {
      if (strncmp(string, string + i, patternLen) != 0)
        isPattern = 0;
    }

    if (isPattern)
      return 1;
  }

  return 0;
}

void challenge2() {
  char buffer[MAX_BUFFER] = {0};

  FILE *file = fopen("input.txt", "r");

  if (!file) {
    perror("Error reading file: ");
    return;
  }

  long sum = 0;

  while(fgets(buffer, MAX_BUFFER - 1, file)) {
    int len = strlen(buffer);
    if (buffer[len - 1] == '\n') buffer[len - 1] = 0;
    
    long lower, upper;

    if (sscanf(buffer, "%ld-%ld", &lower, &upper) != 2) {
      printf("Error: invalid input");
      return;
    }

    for (long i=lower; i <= upper ; i++){
      char *str = intToStr(i);
      if (isInvalid_2(str)) {
        sum += i;
      }
      free(str);
    }
  }


  printf("The sum of invalid ID's is: %ld\n", sum);

}

int main() {
  challenge1();
  challenge2();

  return 0;
}
