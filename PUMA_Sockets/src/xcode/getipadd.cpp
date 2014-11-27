/* getipadd.cpp: ****************************************************

   Program to get the ip address of a hostname.

****************************************************************** */

#include <stdio.h>

#include "msocket.h"

#define SIZE 1024

int main( int argc, char *argv[] )
{
   char ipaddress[SIZE],realhost[SIZE];
   char hostname[SIZE];
   int  errorcode;

   printf ("Hostname (to send data): ");
   scanf  ("%s", &hostname);
   
   if( (errorcode=msocketGetIpAdd( hostname,ipaddress,realhost )) ) {
      printf( "error: Getipadd: %s",msocketGetError( errorcode ) ); 
      return( 1 );
   }
   
   printf( "Hostname  : %s\n",realhost  );
   printf( "IP Address: %s\n",ipaddress );

   return( 0 );
}

/* end of file: getipadd.cpp ***************************************** */

