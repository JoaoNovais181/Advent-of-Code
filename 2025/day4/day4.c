#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include <string.h>

#define MAX_LINE_SIZE 254
#define VALID_THRESHOLD 4

typedef char Matrix[MAX_LINE_SIZE][MAX_LINE_SIZE];

int isNotPaper(Matrix map, size_t len, int i, int j) {
  if (i >= 0 && i < len && j >= 0 && j < len)
    return (map[i][j] == '@') ?1 :0;
  return 0;
}

int isValid(Matrix map, size_t len, int iPos, int jPos) {
  int paperCount = 0;
 
  for (int i=-1 ; i<2 ; i++)
    for (int j=-1 ; j<2 ; j++)
      if (i != 0 || j != 0)
        paperCount += isNotPaper(map, len, iPos + i, jPos + j);

  return paperCount < VALID_THRESHOLD;
}

void printMap(Matrix map, size_t len) {
  printf("\n\n-- Map --\n\n");
  for (int i=0 ; i<len ; i++) {
    for (int j=0 ; j<len ; j++) {
      printf("%c", map[i][j]); 
    }
    puts("");
  }
}

void challenge1() {
  FILE *fp = fopen("input.txt", "r");

  if (!fp) {
    perror("[ERROR] Couldn't open input file: ");
    exit(errno);
  }

  Matrix map;
  size_t idx = 0;
  size_t len = 0;


  while(fgets(map[idx], MAX_LINE_SIZE - 1, fp)) {
    size_t l = strlen(map[idx]);

    if (l==0) break;

    if (map[idx][l - 1] == '\n') map[idx][l - 1] = '\n';
    l--;

    if (!len) len = l;
    else if (len != l) {
      fprintf(stderr, "[ERROR] Error on input file.");
      exit(1);
    }

    idx++;
  }


  int validPapers = 0;

  for (int i=0 ; i<len ; i++)
    for (int j=0 ; j<len ; j++)
      if (map[i][j] == '@')
        validPapers += isValid(map, len, i, j);

  printf("[ Challenge1 ] There are %d valid toilet papers.\n", validPapers);
}

void challenge2() {
  FILE *fp = fopen("input.txt", "r");

  if (!fp) {
    perror("[ERROR] Couldn't open input file: ");
    exit(errno);
  }

  Matrix map;
  size_t idx = 0;
  size_t len = 0;


  while(fgets(map[idx], MAX_LINE_SIZE - 1, fp)) {
    size_t l = strlen(map[idx]);

    if (l==0) break;

    if (map[idx][l - 1] == '\n') map[idx][l - 1] = '\n';
    l--;

    if (!len) len = l;
    else if (len != l) {
      fprintf(stderr, "[ERROR] Error on input file.");
      exit(1);
    }

    idx++;
  }

  int removedPapers = 0;
  int canEnd = 0;

  while(!canEnd) {

    int iPosToRemove[MAX_LINE_SIZE*MAX_LINE_SIZE];
    int jPosToRemove[MAX_LINE_SIZE*MAX_LINE_SIZE];
    int numPosToRemove = 0;

    for (int i=0 ; i<len ; i++)
      for (int j=0 ; j<len ; j++)
        if (map[i][j] == '@' && isValid(map, len, i, j)) {
          iPosToRemove[numPosToRemove] = i;
          jPosToRemove[numPosToRemove] = j;
          numPosToRemove++;
        }

    if (numPosToRemove > 0) {
      for (int i=0 ; i<numPosToRemove ; i++)
        map[iPosToRemove[i]][jPosToRemove[i]] = '.';

      removedPapers += numPosToRemove;
    }
    else canEnd = 1;
  }

  printf("[ Challenge1 ] There are %d toilet papers that can be removed.\n", removedPapers);
}

int main() {
  challenge1();
  challenge2();

  return 0;
}
