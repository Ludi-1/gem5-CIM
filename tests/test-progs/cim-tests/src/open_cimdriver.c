#include <errno.h> // Error integer and strerror() function
#include <fcntl.h> // Contains file controls like O_RDWR
#include <stdio.h>
#include <string.h>
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

int main(void)
{
  int cimfile = open("/dev/cim", O_RDWR);
  printf("%i",cimfile);

  return 0;
}