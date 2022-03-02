/* sio_cmd.c - support library for command-driven serial communications
 *
 * sio_open()           -  open serial port and set mode
 * sio_open_trace()     -  ditto, with trace file
 * sio_cmd()            -  send a command, retrieve response
 */


// #define DEBUG

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
        
#include "sio_cmd.h"

static FILE *tfp = NULL;

static int timeout = 10;	/* timeout (0=none) */

/*
 * open <dev>     (/dev/ttySxx port) for serial I/O
 * return fd or -1 on error
 */
int sio_open( char *dev, int tot) {

  timeout = tot;

  struct termios oldtio,newtio;
  int fd;
        
  /* open device for non-blocking I/O */
  fd = open(dev, O_RDWR | O_NOCTTY);
  //  fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY | O_NONBLOCK); 
  if (fd <0) {
    perror(dev);
    return( -1);
  }
        
  tcgetattr(fd,&oldtio); /* save current port settings */
        
  bzero(&newtio, sizeof(newtio));

  /* set baud rate, 8 bits, no modem control, enable reading */
  newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
  newtio.c_iflag = 0;   /* raw input */
  newtio.c_oflag = 0;   /* raw output */
        
  /* set input mode (non-canonical, no echo,...) */
  newtio.c_lflag = 0;

  /* try to set 1s timeout */
  if( timeout) {
    newtio.c_cc[VTIME] = timeout;	/* timeout deci-sec */
    newtio.c_cc[VMIN] = 0;	/* minimum 0 chars */
  }
         
  /* flush the line and activate new settings */
  tcflush(fd, TCIFLUSH);
  tcsetattr(fd, TCSANOW, &newtio);

  return fd;
}


//
// Send a command string to board, wait for return string terminated by any control character
// Return pointer to static buffer containing return string or NULL on error
//
// if noresp is true, don't expect any response
//
char *sio_cmd( int fd, char *s, int flags)
{
  char cmd;
  int i, res, nc;
  char rch, *p;

  static char buf[SIO_BUF_MAX];

  if( strlen(s) == 0) return NULL;

  // strip any trailing control chars
  for( p = s+strlen(s)-1; iscntrl(*p); --p)
    *p = '\0';
  
  // send the string, followed by \r
  write( fd, s, strlen(s));
  cmd = '\r';
  write( fd, &cmd, 1);

  // if tracing, output the sent string
  if( tfp)
    fprintf( tfp, "Send: %s\n", s);

  if( flags & SIO_NORESP) return NULL;

  // read until we see the prompt
  rch = ' ';
  nc = 0;

  do {
    res = read( fd, &rch, 1);
    
    if( res == 1) {
      buf[nc++] = rch;
      //      printf("RX: %02x\n", rch);
      if( nc == SIO_BUF_MAX) {
	printf("BUffer overflow!\n");
	exit( 1);
      }
    } else {			/* timeout */
      buf[nc++] = '\0';		/* terminate the string */
      return buf;
    }
  } while( (flags & SIO_BINARY) || !iscntrl(rch));
  buf[nc++] = '\0';

  if( rch > 0)
    return( buf);
  else
    return( NULL);
}



// dump a string in hex for debug

void dump_string( char *s)
{
  char *p;

  for( p=s; *p; ++p) {
    printf("[%02x] '%c'\n", *p, *p<0x20 ? '-' : *p);
  }
}
