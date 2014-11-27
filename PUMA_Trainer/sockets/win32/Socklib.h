#include <stdio.h>
#include <IO.h>
#include <winsock.h>

#define QLEN 6

int OpenSocketClient( int portno );
int OpenSocketServer( char *host, int portno );
void CloseSocket( int sd );
int WriteSocket( int sd, char *buf, int size );
int ReadSocket( int sd, char *buf, int size );