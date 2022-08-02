#include <errno.h> // Error integer and strerror() function
#include <fcntl.h> // Contains file controls like O_RDWR
#include <stdint.h> // uint32_t
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h> // ioctl()
#include <sys/mman.h> // mmap()
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

#define WR_VALUE _IOW('a',0x02,uint32_t)
// #define RD_VALUE _IOR('a','b',int32_t*)
#define X_VALUE  _IO('a', 0x01)

int main(void)
{
  //open file
  int fd = open("/dev/cim", O_RDWR);
  printf("/dev/cim open code: %i\n", fd);

  if (fd < 0) {
    printf("Cannot open device file...\n");
    return 0;
  }

  int io1 = ioctl(fd, X_VALUE);
  printf("ioctl1: %i\n", io1);
  printf("cimwrite code: %ld\n", WR_VALUE);

  uint32_t number = 69;
  int io2 = ioctl(fd, WR_VALUE, (uint32_t*) &number);
  printf("ioctl2: %i\n", io2);

  char* cimPtr = mmap(NULL, sizeof(uint32_t),
      PROT_WRITE|PROT_READ, MAP_SHARED, fd, 0);

  const char str1[] = "string 1";
  const char str2[] = "string 2";

  strcpy(cimPtr, str1);
  strcpy((cimPtr+9), str2);

  uint32_t* testPtr = (uint32_t*) (cimPtr + 16);
  *testPtr = 21;

  uint32_t value = *(uint32_t*) (cimPtr + 16);
  printf("text: %u\n", value);

  //close file
  close(fd);

  return 0;
  }