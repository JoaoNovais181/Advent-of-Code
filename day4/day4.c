#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_READ_BUFFER 1024

static char read_buffer[MAX_READ_BUFFER];
static int read_buffer_pos = 0;
static int read_buffer_end = 0;

int readC (int fd, char *c)
{
    // 1 char de cada vez
    // return read(fd, c, 1);

    if (read_buffer_pos == read_buffer_end)
    {
        read_buffer_end = read(fd, read_buffer, MAX_READ_BUFFER);
        switch(read_buffer_end)
        {
            case -1:
                return -1;
                break;
            case 0:
                return 0;
                break;
            default:
                read_buffer_pos = 0;
        }
    }

    *c = read_buffer[read_buffer_pos++];

    return 1;
}


ssize_t readln(int fd, char *buf, size_t size)
{
    int i = 0;

    while ( i<size && readC(fd, &buf[i])>0)
        if (buf[i++] == '\n') return i;

    return i;
}

// estruturas e funcionalidades delas

typedef struct matrix {
  int numCols, numLines;
  char **matrix;
} Matrix;

typedef struct x_mas {
  int iMin, iMax, jMin, jMax;
} X_MAS;

typedef struct x_mas_list {
  X_MAS *array;
  size_t capacity, used;
} *X_MAS_LIST;

void freeMatrix (Matrix matrix) {
  for (int i=0 ; i<matrix.numLines ; i++)
    free(matrix.matrix[i]);

  free(matrix.matrix);
}

Matrix readInput() {
  int fd = open("input.txt", O_RDONLY);

  if (fd == -1)
    exit(errno);

  Matrix matrix = {0, 0, NULL};
  matrix.matrix = malloc(0);
  if (matrix.matrix == NULL) {
    perror("Memory allocation error");
    exit(errno);
  }
  
  char buffer[MAX_READ_BUFFER];

  while (readln(fd, buffer, MAX_READ_BUFFER) > 0) {

    matrix.numLines++;

    char **new_matrix = realloc(matrix.matrix, matrix.numLines * sizeof(char *));
    if (!new_matrix) {
      perror("Memory allocation error");
      freeMatrix(matrix);
      exit(EXIT_FAILURE);
    }
    matrix.matrix = new_matrix;

    // copiar a linha lida para a matriz, alocando a memoria (strdup aloca a
    // memoria)
    buffer[strlen(buffer) - 1] = 0; // ignorar o \n
    matrix.matrix[matrix.numLines - 1] = strdup(buffer);
    matrix.numCols = strlen(buffer);
  }

  if (close(fd) == -1) {
    perror("Error closing file");
  }

  return matrix;
}

X_MAS_LIST xmasListInit (size_t capacity) {
  X_MAS_LIST list = malloc(sizeof(struct x_mas_list));
  list->used = 0;
  list->capacity = capacity;
  list->array = malloc(sizeof(X_MAS) * capacity);

  return list;
}

void xmasListAppend (X_MAS_LIST list, X_MAS elem) {
  if (list->used >= list->capacity) {
    list->capacity *= 1.5;
    list->array = realloc(list->array, list->capacity * sizeof(X_MAS));
  } 

  list->array[list->used++] = elem; 
}

int xmasListFind (X_MAS_LIST list, X_MAS elem) {
  for (int i = 0 ; i < list->used ; i++) {
    X_MAS x = list->array[i];
    if (elem.iMin == x.iMin && elem.jMin == x.jMin &&
        elem.iMax == x.iMax && elem.jMax == x.jMax)
      return 1;
  }
  return 0;
}

void xmasListFree (X_MAS_LIST list) {
  free(list->array);
  free(list);
}

int search(int iPos, int jPos, int iDir, int jDir, Matrix input) {

  if (iDir == 0 && jDir == 0)
    return 0;

  const char targets[] = "XMAS";
  int target = 0;

  for (int i = iPos, j = jPos;
       i < input.numLines && j < input.numCols && target < 4 && i >= 0 && j>= 0;
       i += iDir, j += jDir, target++) {

    if (input.matrix[i][j] != targets[target])
      return 0;

  }

  return target == 4;
}

void challenge1(Matrix input) {
  int r = 0;
  
  for (int i = 0 ; i < input.numLines ; i++) {
    for (int j = 0 ; j < input.numCols ; j++) {

      if (input.matrix[i][j] != 'X')
        continue;
      
      for (int iDir = -1 ; iDir <= 1 ; iDir++)
        for (int jDir = -1 ; jDir <= 1 ; jDir++)
          r += search(i, j, iDir, jDir, input);
    }
  }
  printf("Challenge1: %d\n", r);
}

X_MAS searchTopLeft (int iPos, int jPos, Matrix input) {
  X_MAS x = {-1, -1, -1, -1};
  char mas[4] = "MAS";

  if (iPos < input.numLines-2 && jPos < input.numCols-2) {
    char mas1[4], mas2[4], mas3[4];
    mas1[3] = mas2[3] = mas3[3] = 0;
    mas1[0] = input.matrix[iPos][jPos];
    mas1[1] = input.matrix[iPos+1][jPos+1];
    mas1[2] = input.matrix[iPos+2][jPos+2];

    mas2[0] = input.matrix[iPos+2][jPos];
    mas2[1] = input.matrix[iPos+1][jPos+1];
    mas2[2] = input.matrix[iPos][jPos+2];

    mas3[0] = input.matrix[iPos][jPos+2];
    mas3[1] = input.matrix[iPos+1][jPos+1];
    mas3[2] = input.matrix[iPos+2][jPos];

    if (strcmp(mas1, mas) == 0 && (strcmp(mas2, mas) == 0 || strcmp(mas3, mas) == 0)) {
      x.iMin = iPos;
      x.iMax = iPos + 2;
      x.jMin = jPos;
      x.jMax = jPos + 2;
    }
  }

  return x;
}

X_MAS searchTopRight (int iPos, int jPos, Matrix input) {
  X_MAS x = {-1, -1, -1, -1};
  char mas[4] = "MAS";

  if (iPos < input.numLines-2 && jPos > 1) {
    char mas1[4], mas2[4], mas3[4];
    mas1[3] = mas2[3] = mas3[3] = 0;
    mas1[0] = input.matrix[iPos][jPos];
    mas1[1] = input.matrix[iPos+1][jPos-1];
    mas1[2] = input.matrix[iPos+2][jPos-2];

    mas2[0] = input.matrix[iPos+2][jPos];
    mas2[1] = input.matrix[iPos+1][jPos-1];
    mas2[2] = input.matrix[iPos][jPos-2];

    mas3[0] = input.matrix[iPos][jPos-2];
    mas3[1] = input.matrix[iPos+1][jPos-1];
    mas3[2] = input.matrix[iPos+2][jPos];

    if (strcmp(mas1, mas) == 0 && (strcmp(mas2, mas) == 0 || strcmp(mas3, mas) == 0)) {
      x.iMin = iPos;
      x.iMax = iPos + 2;
      x.jMin = jPos - 2;
      x.jMax = jPos;
    }
  }

  return x;
}

X_MAS searchBotLeft (int iPos, int jPos, Matrix input) {
  X_MAS x = {-1, -1, -1, -1};
  char mas[4] = "MAS";

  if (iPos > 1 && jPos < input.numCols-2) {
    char mas1[4], mas2[4], mas3[4];
    mas1[3] = mas2[3] = mas3[3] = 0;
    mas1[0] = input.matrix[iPos][jPos];
    mas1[1] = input.matrix[iPos-1][jPos+1];
    mas1[2] = input.matrix[iPos-2][jPos+2];

    mas2[0] = input.matrix[iPos-2][jPos];
    mas2[1] = input.matrix[iPos-1][jPos+1];
    mas2[2] = input.matrix[iPos][jPos+2];

    mas3[0] = input.matrix[iPos][jPos+2];
    mas3[1] = input.matrix[iPos-1][jPos+1];
    mas3[2] = input.matrix[iPos-2][jPos];

    if (strcmp(mas1, mas) == 0 && (strcmp(mas2, mas) == 0 || strcmp(mas3, mas) == 0)) {
      x.iMin = iPos - 2;
      x.iMax = iPos;
      x.jMin = jPos;
      x.jMax = jPos + 2;
    }
  }

  return x;
}

X_MAS searchBotRight (int iPos, int jPos, Matrix input) {
  X_MAS x = {-1, -1, -1, -1};
  char mas[4] = "MAS";

  if (iPos > 1 && jPos > 1) {
    char mas1[4], mas2[4], mas3[4];
    mas1[3] = mas2[3] = mas3[3] = 0;
    mas1[0] = input.matrix[iPos][jPos];
    mas1[1] = input.matrix[iPos-1][jPos-1];
    mas1[2] = input.matrix[iPos-2][jPos-2];

    mas2[0] = input.matrix[iPos-2][jPos];
    mas2[1] = input.matrix[iPos-1][jPos-1];
    mas2[2] = input.matrix[iPos][jPos-2];

    mas3[0] = input.matrix[iPos][jPos-2];
    mas3[1] = input.matrix[iPos-1][jPos-1];
    mas3[2] = input.matrix[iPos-2][jPos];

    if (strcmp(mas1, mas) == 0 && (strcmp(mas2, mas) == 0 || strcmp(mas3, mas) == 0)) {
      x.iMin = iPos - 2;
      x.iMax = iPos;
      x.jMin = jPos - 2;
      x.jMax = jPos;
    }
  }

  return x;
}

void challenge2(Matrix input) {
  X_MAS_LIST list = xmasListInit(100);

  for (int i = 0 ; i < input.numLines ; i++)
    for (int j = 0 ; j < input.numCols ; j++) {
      if (input.matrix[i][j] != 'M')
        continue;
      X_MAS x[4];
      x[0] = searchTopLeft(i,j,input);
      x[1] = searchTopRight(i,j,input);
      x[2] = searchBotLeft(i,j,input);
      x[3] = searchBotRight(i,j,input);

      for (int k=0 ; k<4 ; k++)
        if (x[k].iMin != -1 && !xmasListFind(list, x[k]))
          xmasListAppend(list, x[k]);
    }

  printf("Challenge2: %lu\n", list->used);
}


int main() { 
  Matrix input = readInput(); 
  
  challenge1(input);
  challenge2(input);

  freeMatrix(input);
}
