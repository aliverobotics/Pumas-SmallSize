/* msocket.cpp: ************************************************************

   My Socket Handler functions library implementation.
   
   28/April/2003: Hiram Galicia. Version 2.0

   NOTES:

 - Based on gsNetPoll library
 - Object Oriented Programming

************************************************************************* */

/* standard c libraries */

   #include <string.h>
   #include <stdlib.h>
   #include <stdio.h>

/* standard c libraries for sockets management */


   #include "stdafx.h"
   #include <winsock.h>
   #include <fcntl.h>
   #include "Socklib.h"

   #include <sys/types.h>

/* user libraries */
 
   #include "msocket.h"

/* msocketInitSocket: ******************************************************

   msocketInitSocket returns MSOCKET_OK (i.e. 0) or the errorcode.

   hostname   : mezcal.fi-b.unam.mx, tx8.fi-b.unam.mx, www.yahoo.com, 132.248.59.97, ...
   port       : 2010, 3234, 8230, ...
   mode       : RX_SOCKET, TX_SOCKET

   call example:

   msocketData msd;

   msd.msocketInitSocket( "mezcal.fi-b.unam.mx",5000,TX_SOCKET )
   
************************************************************************* */

int msocketData::msocketInitSocket( char *hostname,int port,int Mode )
{

	// check the socket mode: RX_SOCKET,TX_SOCKET 

   if( Mode<RX_SOCKET || Mode>TX_SOCKET ) return( MSOCKET_INVALID_MODE );
   mode = Mode;		// set the socket mode 


   if( mode==RX_SOCKET )
   {
		if ((sd=OpenSocketServer(port))<0)
			return( MSOCKET_IMPOSSIBLE_TO_BIND );
   } 
   else
   {    
		if ((sd=OpenSocketClient( hostname, port ))<0)
			return( MSOCKET_IMPOSSIBLE_TO_BIND );
   }

   return( MSOCKET_OK ); /* succesfull socket creation */ 
}

void msocketData::msocketKillSocket( void )
{
   CloseSocket( sd );
}

int msocketData::msocketRxData( char *buffer,int buffersize )
{
   int n;
   if( mode != RX_SOCKET ) return( MSOCKET_RX_ATTEMPT_IN_TX_SOCK );
 
   n = ReadSocket( sd, buffer, buffersize );
   return( n );
}

int msocketData::msocketTxData( char *buffer,int buffersize )
{
   if( mode != TX_SOCKET ) return( MSOCKET_TX_ATTEMPT_IN_RX_SOCK );

   if( WriteSocket( sd, buffer ) < 0 )
      return( MSOCKET_CAN_NOT_TX );

   return( MSOCKET_OK );
}

int msocketGetIpAdd( char *hostname,char *ipaddress,char *realhost )
{
   return 0;
}

char *msocketGetError( int errorcode )
{
   switch( errorcode ) {
      case MSOCKET_OK:
         return( "msocket: Socket Ok: there is no problem." );
      case MSOCKET_INVALID_MODE:
         return( "msocket: msocketInitSocket: invalid mode opening socket." );
      case MSOCKET_CAN_NOT_CREATE:
         return( "msocket: msocketInitSocket: can not create the socket." );
      case MSOCKET_IPADDR_NOT_FOUND:
         return( "msocket: msocketInitSocket: can not found the ipaddress of that hostname." );
      case MSOCKET_IMPOSSIBLE_TO_BIND:
         return( "msocket: msocketInitSocket: can not bind name to datagram socket." );
      case MSOCKET_RX_ATTEMPT_IN_TX_SOCK:
         return( "msocket: msocketRxData: attempting to rx in a tx socket." );
      case MSOCKET_TX_ATTEMPT_IN_RX_SOCK:
         return( "msocket: msocketTxData: attempting to tx in a rx socket." );
      case MSOCKET_CAN_NOT_TX:
         return( "msocket: msocketTxData: can not send datagram." );
      case MSOCKET_IPADDR_NOT_FOUND_GIA:
         return( "msocket: msocketGetIpAdd: can not found the ipaddress of that hostname." );
      default:
         return( "msocket: unknown error." );
   }

	return NULL;
}

/* end of file: msocket.cpp ********************************************* */

