
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>

#include "sio_cmd.h"

int main()
{
  char buff[255];
  char *s;
  int fd;
  
  if( (fd = sio_open("/dev/ttyUSB0", 10)) < 0) {
    printf("Error opening serial port\n");
    exit( 1);
  }

  while( 1) {
    printf("cmd>");
    fgets( buff, 255, stdin);

    if( toupper( *buff) == 'Q')
      break;

    printf("Sending cmd: %s\n", buff);
    s = sio_cmd( fd, buff, 0);
    if( s == NULL)
      printf("sio_cmd() error\n");
    else {
      printf("Got %ld chars\n", strlen( s));
      dump_string( s);
    }
  }

  close( fd);
  
}
