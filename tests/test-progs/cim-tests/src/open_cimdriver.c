#include <errno.h> // Error integer and strerror() function
#include <fcntl.h> // Contains file controls like O_RDWR
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h> // mmap()
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

int main(void)
{
  int cimfile = open("/dev/cim", O_RDWR);
  // printf("%i",cimfile);
  char* cimPtr = mmap(NULL, sizeof(uint32_t),
    PROT_WRITE|PROT_READ, MAP_SHARED, cimfile, 0);
  for (int i = 0; i < 100; i++){
    *(cimPtr + i) = 1;
  }
  return 0;
}