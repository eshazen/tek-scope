//
// read ascii waveform from specified channel
//
// usage: read_wf <ch> <start> <stop>
//

#include "sio_cmd.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


// decode '#nddd' format header
//   return pointer to start of data
//   set dlen to length
char *header_decode( char *s, int *dlen) {
  int n;
  int v = 0;
  if( *s != '#' || !isdigit(s[1])) return NULL;
  for( int i=0; i<(s[1]-'0'); i++) {
    if( !isdigit( s[2+i])) return NULL;
    v = v*10 + (s[2+i]-'0');
  }
  *dlen = v;
  return &s[2+s[1]-'0'];
}


int main( int argc, char *argv[]) {

  char *ch = "CH1";
  char *start = "1";
  char *stop = "500";

  char buff[255];
  char *s;
  int fd;

  FILE *fp;
  int dlen;
  char *p;
  
  if( argc > 1)
    ch = argv[1];

  if( argc > 2)
    start = argv[2];

  if( argc > 3)
    stop = argv[3];

  printf("# ch=%s start=%s stop=%s\n", ch, start, stop);

  if( (fd = sio_open("/dev/ttyUSB0", 10)) < 0) {
    printf("Error opening serial port\n");
    exit( 1);
  }

  s = sio_cmd( fd, "*IDN?", 0);

  printf("# ID: %s\n", s);

  sprintf( buff, "DAT:SOU %s", ch);
  printf("# Set source: %s\n", buff);
  s = sio_cmd( fd, buff, SIO_NORESP);

  s = sio_cmd( fd, "DAT:SOU?", 0);
  printf("# Confirm source: %s\n", s);

  //  s = sio_cmd( fd, "DAT:ENC ASCI", SIO_NORESP);
  s = sio_cmd( fd, "DAT:ENC SRI", SIO_NORESP);  
  s = sio_cmd( fd, "DAT:ENC?", 0);
  printf("# Confirm encoding: %s\n", s);

  sprintf( buff, "DAT:STAR %s", start);
  s = sio_cmd(fd, buff, SIO_NORESP);

  sprintf( buff, "DAT:STOP %s", stop);
  s = sio_cmd( fd, buff, SIO_NORESP);

  s = sio_cmd( fd, "CURV?", 0);

//  // write to a gnuplot file
//  if( (fp = fopen( "plot.dat", "w")) == NULL) {
//    printf("Error opening plot.dat for output\n");
//    exit(1);
//  }

  // look at header
  if( (p = header_decode( s, &dlen)) == NULL) {
    printf("Error decoding binary header\n");
    exit(1);
  }

  printf("# Binary data length: %d\n", dlen);

  for( int i=0; i<dlen; i++) {
    printf("%d %d\n", i, p[i]);
  }

}
