// myUdpSockets.cpp
/*

#include "stdafx.h"
#include "myUdpSockets.h"

#define BUFF_SIZE 1024

UDPSockets::UDPSockets( void  )
{
}

int UDPSockets::UDPSocketsCreate( char *Host, int Port )
{
	sprintf( host, "%s", Host );
	port = Port;

	Error = CreateUDPSocket(client,host,port,UDP_CLIENT);
	return Error;
}

int UDPSockets::UDPSocketsWrite( char *msg )
{
	Error = WriteUDPSox(client,msg,BUFF_SIZE);
	return Error;
}

*/

///adal

// myUdpSockets.cpp


#include "stdafx.h"
#include "myUdpSockets.h"

#define BUFF_SIZE 1024

UDPSockets::UDPSockets( void  )
{
}

int UDPSockets::UDPSocketsCreate( char *Host, int Port )
{
	sprintf( host, "%s", Host );
	port = Port;

	Error = client.msocketInitSocket(host,port,TX_SOCKET);
	return Error;
}

int UDPSockets::UDPSocketsWrite( char *msg )
{
	Error = client.msocketTxData(msg,BUFF_SIZE);
	return Error;
}
