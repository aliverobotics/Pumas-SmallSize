#include <stdio.h>
#include <IO.h>
#include <winsock.h>

#define QLEN 6

int OpenSocketServer( int portno );
int OpenSocketClient( char *host, int portno );
void CloseSocket( int sd );
int WriteSocket( int sd, char *buf );
int ReadSocket( int sd, char *buf, int size );