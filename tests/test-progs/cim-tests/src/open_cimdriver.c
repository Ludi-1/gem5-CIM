#include <errno.h> // Error integer and strerror() function
#include <fcntl.h> // Contains file controls like O_RDWR
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h> // mmap()
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

int main(void)
{
  int cimfile = open("/dev/cim", O_RDWR);
  printf("cim file output: %i\n",cimfile);
  int* cimPtr = mmap(NULL, sizeof(int),
    PROT_WRITE|PROT_READ, MAP_SHARED, cimfile, 0);

  *(cimPtr + 0) = -300;
  printf("1st ptr value: %d\n", *(cimPtr + 0));
  *(cimPtr + 1) = 1;
  int a = 0;
  int b = -129;
  int c = a + b;
  *(cimPtr + 2) = a;
  *(cimPtr + 2) = b;
  *(cimPtr + 2) = c;
  *(cimPtr + 3) = 4;

  int num1 = *(cimPtr + 2);
  int num2 = *(cimPtr + 3);
  printf("c: %" PRId32 "\n", c);
  printf("num1: %d num2: %d\n", num1, num2);
  return 0;
}