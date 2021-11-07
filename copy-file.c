// apk add gcc gcompat strace musl-dev
#define _GNU_SOURCE
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
  char* infile = getenv("IN_FILE");
  char* outfile = getenv("OUT_FILE");

  int fd = open(infile, O_RDONLY|O_LARGEFILE|O_CLOEXEC);
  int fd2 = open(outfile,  O_WRONLY|O_CREAT|O_LARGEFILE|O_CLOEXEC, 0100644);

  int bytes_written = copy_file_range(fd, 0, fd2, NULL, 12, 0);
  printf("Bytes written %d \n", bytes_written);

  close(fd);
  close(fd2);

  return 0;
}
