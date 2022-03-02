#ifndef _SIO_CMD_H

#define SIO_BUF_MAX 100000        

// #define BAUDRATE B9600
#define BAUDRATE B38400
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1

int sio_open( char *dev, int to);
char *sio_cmd( int fd, char *s, int flags);
void dump_string( char *s);
int sio_open_trace( char *dev, char *tracef);

// flags for sio_cmd()
// don't expect response
#define SIO_NORESP 1
// binary mode (no special terminator, wait for timeout)
#define SIO_BINARY 2
// binary with prefix #dxxx
#define SIO_PREBIN 4

#define _SIO_CMD_H
#endif

