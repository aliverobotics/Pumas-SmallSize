// myUdpSockets.h

/*
#include "sockets.h"

class UDPSockets
{
  private:
	int				Error;
	SoxManager		client;
	char			msg[256];
	char			host[100],portS[10];
	int				port;

  public:
	UDPSockets( void );
	int UDPSocketsCreate( char *, int );
	int UDPSocketsWrite( char * );
};

*/


#include "msocket.h"

class UDPSockets
{
  private:
	int				Error;
	msocketData		client;
	char			msg[256];
	char			host[100],portS[10];
	int				port;

  public:
	UDPSockets( void );
	int UDPSocketsCreate( char *, int );
	int UDPSocketsWrite( char * );
};


