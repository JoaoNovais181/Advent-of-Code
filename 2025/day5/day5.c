#include <assert.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>

#define MAX_LINE_SIZE 128

typedef struct {
 long lower, upper;
} *Range;

typedef struct {
  size_t currentSize, maxSize;
  Range *list;
} *FreshRanges;

typedef struct {
  size_t currentSize, maxSize;
  long *list;
} *Products;

typedef struct {
  FreshRanges freshRanges;
  Products products;
} ProblemInput;

Range allocRange(long lower, long upper) {
  Range range = malloc(sizeof(*range));
  range->lower = lower;
  range->upper = upper;

  return range;
} 

FreshRanges allocFreshRanges(size_t initialSize) {
  FreshRanges freshRanges = malloc(sizeof(*freshRanges));
  freshRanges->currentSize = 0;
  freshRanges->maxSize = initialSize;
  freshRanges->list = malloc(initialSize * sizeof(Range));

  return freshRanges;
}

void appendRange(FreshRanges freshRanges, Range range) {
  if (freshRanges->currentSize == freshRanges->maxSize) {
    freshRanges->maxSize = freshRanges->maxSize * 3 / 2;
    freshRanges->list = realloc(freshRanges->list, freshRanges->maxSize * sizeof(Range));
  }
  freshRanges->list[freshRanges->currentSize++] = range;
}

Products allocProducts(size_t initialSize) {
  Products products = malloc(sizeof(*products));
  products->currentSize = 0;
  products->maxSize = initialSize;
  products->list = malloc(initialSize * sizeof(long));

  return products;
}

void appendProduct(Products products, long product) {
  if (products->currentSize == products->maxSize) {
    products->maxSize = products->maxSize * 3 / 2;
    products->list = realloc(products->list, products->maxSize * sizeof(long));
  }
  products->list[products->currentSize++] = product;
}

void printFreshRanges(FreshRanges freshRanges) {
  for (int i=0 ; i<freshRanges->currentSize ; i++) {
    printf("%ld-%ld\n", freshRanges->list[i]->lower, freshRanges->list[i]->upper);
  }
  puts("");
}

void printProblemInput(ProblemInput input) {
  for (int i=0 ; i<input.freshRanges->currentSize ; i++) {
    printf("%ld-%ld\n", input.freshRanges->list[i]->lower, input.freshRanges->list[i]->upper);
  }
  puts("");
  for (int i=0 ; i<input.products->currentSize ; i++) {
    printf("%ld\n", input.products->list[i]);
  }
}

ProblemInput parse_input(char *file) {
  FILE *fp = fopen(file, "r");

  if (!fp) {
    perror("[ERROR] Couldn't open input file: ");
    exit(errno);
  }

  char buffer[MAX_LINE_SIZE];
  int readingRanges = 1;

  ProblemInput r;
  r.freshRanges = allocFreshRanges(100);
  r.products = allocProducts(100);

  while(fgets(buffer, MAX_LINE_SIZE - 1, fp)) {
    if (strlen(buffer) == 1) {
      readingRanges = 0;
      continue;
    }

    if (readingRanges) {
      long lower, upper;
      if (sscanf(buffer, "%ld-%ld\n", &lower, &upper) != 2) {
        fprintf(stderr, "[Error] Malformed input.");
        exit(1);
      }
      appendRange(r.freshRanges, allocRange(lower, upper));
    }
    else {
      long product;
      if (sscanf(buffer, "%ld\n", &product) != 1) {
        fprintf(stderr, "[Error] Malformed input.");
        exit(1);
      }
      appendProduct(r.products, product);
    }
  }

  return r;
}

void challenge1(ProblemInput input) {
  long freshProductCount = 0;

  for (int i=0 ; i<input.products->currentSize ; i++) {
    int fresh = 0;
    long product = input.products->list[i];
    for (int j=0 ; j<input.freshRanges->currentSize ; j++) {
      Range range = input.freshRanges->list[j];

      if (range->lower <= product && product <= range->upper) {
        fresh = 1;
        break;
      }
    }
    freshProductCount += fresh;
  }

  printf("[Challenge1] There are %ld fresh products.\n", freshProductCount);
}

void sortFreshRanges(FreshRanges freshRanges) {
  for (size_t i=0 ; i<freshRanges->currentSize ; i++) {
    Range max = freshRanges->list[0];
    size_t maxIdx = 0;

    for(size_t j=1 ; j<freshRanges->currentSize - i ; j++)
      if (freshRanges->list[j]->lower > max->lower) {
        max = freshRanges->list[j];
        maxIdx = j;
      }

    Range tmp = freshRanges->list[freshRanges->currentSize - i - 1];
    freshRanges->list[freshRanges->currentSize - i - 1] = max;
    freshRanges->list[maxIdx] = tmp;
  }
}

int mergeRanges(Range r1, Range r2) {
  assert(r1->lower <= r2->lower);

  if (r2->lower > r1->upper)
    return 0;

  r1->upper = (r1->upper > r2->upper) ?r1->upper :r2->upper;

  return 1;
}

void challenge2(ProblemInput input) {
  FreshRanges freshRanges = input.freshRanges;

  sortFreshRanges(freshRanges);
  
  FreshRanges mergedRanges = allocFreshRanges(100);
  Range lower = freshRanges->list[0];
  Range result = allocRange(lower->lower, lower->upper);
  // merge overlapping ranges
  for (size_t i=1 ; i<freshRanges->currentSize ; i++) {
    Range upper = freshRanges->list[i];

    int merged = mergeRanges(result, upper);
    if (!merged || i==freshRanges->currentSize - 1)
      appendRange(mergedRanges, result);
    if (!merged)
      result = allocRange(upper->lower, upper->upper);
  }

  long freshProductCount = 0;
  for (size_t i=0 ; i<mergedRanges->currentSize ; i++)
    freshProductCount += (mergedRanges->list[i]->upper - mergedRanges->list[i]->lower + 1);

  printf("[Challenge2] There are %ld fresh products.\n", freshProductCount);
}

int main() {
  ProblemInput input = parse_input("input.txt");
  challenge1(input);
  challenge2(input);

  return 0;
}
