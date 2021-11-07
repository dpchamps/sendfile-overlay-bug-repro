#define _GNU_SOURCE
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

void create_file(char* f) {
  int fd = open(f, O_WRONLY| O_CREAT | O_EXCL);
  char* msg = "write some bytes";

  write(fd, msg, 16);
  close(fd);
}

int copy_with_sendfile(char* from, char* to) {
  int fd1 = open(from, O_RDONLY);    
  struct stat fd1_info;                
  fstat(fd1, &fd1_info);               
  int fd2 = open(to, O_WRONLY | O_CREAT|O_EXCL,  0100644);

  int bytes = sendfile(fd2, fd1, NULL, fd1_info.st_size); 

  close(fd1);
  close(fd2);                                                                

  return bytes;         
}

int copy_with_cfr(char* from, char* to) {
  int fd1 = open(from, O_RDONLY);                         
  struct stat fd1_info;                                   
  fstat(fd1, &fd1_info);                                  
  int fd2 = open(to, O_WRONLY | O_CREAT|O_EXCL,  0100644);
                                                          
  int bytes = copy_file_range(fd1, 0, fd2, NULL, fd1_info.st_size, 0);

  close(fd1);                                                                                                                                           
  close(fd2);

  return bytes;                                                  
}


int main() {
  create_file("test");  

  int og_sf_bw = copy_with_sendfile("test", "test-sf");
  int sf_cfr_bw = copy_with_cfr("test-sf", "test-cfr");
  int og_cfr_bw = copy_with_cfr("test", "test-cfr-og");
  int cfr_cfr_bw = copy_with_cfr("test-cfr-og", "test-cfr-cfr");
  
  printf("\tsendfile bytes written: %d\n\tsendfile -> copy_file_range bytes written: %d\n\tcopy_file_range bytes written: %d\n\tcfr->cfr bytes written: %d\n", 
        og_sf_bw, 
        sf_cfr_bw,
        og_cfr_bw,
        cfr_cfr_bw);

  return 0;
}