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
    ssize_t i = 0;
    int r;
    while (i < size) {
        r = readC(fd, &buf[i]);
        if (r < 0) return -1;  // Error
        if (r == 0) break;     // EOF
        if (buf[i++] == '\n') return i;
    }
    return i;
}

void cleanRead() {
  read_buffer_pos = 0;
  read_buffer_end = 0;
  read_buffer[0] = 0;
}

void challenge1() {
  int fd = open("input.txt", O_RDONLY);

  if (fd == -1)
    exit(errno);

  int pos = 50;
  int zeroCount = 0;

  char buffer[MAX_READ_BUFFER];

  while (readln(fd, buffer, MAX_READ_BUFFER) > 0) {
    char dir;
    int num;

    if (sscanf(buffer, "%c%d\n", &dir, &num) != 2)
    {
      printf("[Error] wrong input format");
    }

    pos = (100 + pos + ((dir == 'R') ?num :-num)) % 100;
    
    if (pos == 0)
      zeroCount++;
  }

  printf("1: The Password is %d\n", zeroCount);

  if (close(fd) == -1) {
    perror("Error closing file");
  }
}

void challenge2() {
  int fd = open("input.txt", O_RDONLY);

  if (fd == -1)
    exit(errno);

  int pos = 50;
  int zeroCount = 0;

  char buffer[MAX_READ_BUFFER];

  while (readln(fd, buffer, MAX_READ_BUFFER) > 0) {
    char dir;
    int num;

    if (sscanf(buffer, "%c%d\n", &dir, &num) != 2)
    {
      printf("[Error] wrong input format");
    }
  
    zeroCount += num / 100;
    num = num % 100;

    if (pos != 0 && 
        (
          (dir == 'R' && pos + num > 100) 
          || 
          (dir == 'L' && pos - num < 0)
        )
    )
      zeroCount += 1;

    pos = (100 + pos + ((dir == 'R') ?num :-num)) % 100;

    if (pos == 0)
      zeroCount += 1;
  }

  printf("2: The Password is %d\n", zeroCount);

  if (close(fd) == -1) {
    perror("Error closing file");
  }
}

int main() {
  // challenge1();
  // cleanRead();
  challenge2();

  return 0;
}
