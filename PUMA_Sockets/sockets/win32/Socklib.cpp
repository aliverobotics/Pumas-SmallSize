#include "stdafx.h"
#include "Socklib.h"
#include <winsock.h>

/* Convert host name to equivalent IP address and copy to sad. */
char *GetHostADDR ( char *host )
{
	struct  hostent  *ptrh;  /* pointer to a host table entry       */

	/* Check host argument and assign host name. */

	if (host==NULL || host == "") 
	   host = "localhost";

	ptrh = gethostbyname(host);

	if ( ((char *)ptrh) == NULL ) 
		fprintf(stderr,"invalid host: %s\n", host);

	return (char *)ptrh;
}

int OpenSocketClient( int port )
{
	struct  sockaddr_in sad; /* structure to hold server's address  */
    int     sd;				 /* socket descriptors                  */
	char	host_name[256];	 /* Host name of this computer */
	struct	hostent *hp;	 /* Information about the server */

	WSADATA wsaData;

	/* Open windows connection */
	if (WSAStartup(0x0101, &wsaData) != 0)
	{
		fprintf(stderr, "Could not open Windows connection.\n");
		return(-1);
	}

	/* Open a datagram socket */
	sd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sd == INVALID_SOCKET)
	{
		fprintf(stderr, "Could not create socket.\n");
		WSACleanup();
		return (-1);
	}

	/* Clear out server struct */
	memset((void *)&sad, '\0', sizeof(struct sockaddr_in));

	/* Set family and port */
	sad.sin_family = AF_INET;
	sad.sin_port = htons(port);
 
	/* Get host name of this computer */
	gethostname(host_name, sizeof(host_name));
	hp = gethostbyname(host_name);

	/* Check for NULL pointer */
	if (hp == NULL)
	{
		fprintf(stderr, "Could not get host name.\n");
		closesocket(sd);
		WSACleanup();
		return (-1);
	}

	/* Assign the address */
	sad.sin_addr.S_un.S_un_b.s_b1 = hp->h_addr_list[0][0];
	sad.sin_addr.S_un.S_un_b.s_b2 = hp->h_addr_list[0][1];
	sad.sin_addr.S_un.S_un_b.s_b3 = hp->h_addr_list[0][2];
	sad.sin_addr.S_un.S_un_b.s_b4 = hp->h_addr_list[0][3];

	/* Bind local address to socket */
	if (bind(sd, (struct sockaddr *)&sad, sizeof(struct sockaddr_in)) == -1)
	{
		fprintf(stderr, "Cannot bind address to socket.\n");
		closesocket(sd);
		WSACleanup();
		return(-1);
	}

	/* setting the unblocked */
	unsigned long nonblocked = 1;
	int error = ioctlsocket(sd, FIONBIO, &nonblocked);

	/* unsetting blocking mode */
	if (error<0)
	{
		fprintf(stderr, "Cannot make unblocking to socket => %d.\n", error);
		closesocket(sd);
		WSACleanup();
		return(-1);
	}

	return sd;
}

int OpenSocketServer( char *host, int port )
{
	struct  sockaddr_in sad; /* structure to hold an IP address     */
	int     sd;              /* socket descriptor                   */
	int a1, a2, a3, a4;		 /* Server address components in xxx.xxx.xxx.xxx form */

	if (sscanf(host, "%d.%d.%d.%d", &a1, &a2, &a3, &a4) != 4)
	{
		fprintf(stderr, "Address incomplete.\n");
		WSACleanup();
		return (-1);
	}

	WSADATA wsaData;
	WSAStartup(0x0101, &wsaData);

	memset((char *)&sad,0,sizeof(sad)); /* clear sockaddr structure */
	sad.sin_family = AF_INET;         /* set family to Internet     */

	/* Check command-line argument for protocol port and extract    */
	/* port number if one is specified.  Otherwise, use the default */
	/* port value given by constant PROTOPORT                       */
	sad.sin_port = htons((u_short)port);

	/* Set server address */
	sad.sin_addr.S_un.S_un_b.s_b1 = (unsigned char)a1;
	sad.sin_addr.S_un.S_un_b.s_b2 = (unsigned char)a2;
	sad.sin_addr.S_un.S_un_b.s_b3 = (unsigned char)a3;
	sad.sin_addr.S_un.S_un_b.s_b4 = (unsigned char)a4;

	/* Open a datagram socket */
	sd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sd == INVALID_SOCKET)
	{
		fprintf(stderr, "Could not create socket.\n");
		WSACleanup();
		return (-1);
	}

	/* Connect the socket to the specified server. */
	if (connect(sd, (struct sockaddr *)&sad, sizeof(sad)) < 0) {
		printf("connect failed \n");
		return -1;
	}

	return(sd);
}

void CloseSocket( int sd )
{
	close( sd );
}

int WriteSocket( int sd, char *buf, int size)
{
	int n;
	n = send(sd,buf,size,0);
	return n;
}

int ReadSocket( int sd, char *buf, int size )
{
	int n;

	/* Receive time */
	n = recv(sd, buf, size, 0);
	return n;
}

int GettLocalIPAddress (char* addr)
{
	struct  sockaddr_in sad; /* structure to hold server's address  */
	char	host_name[256];	 /* Host name of this computer */
	struct	hostent *hp;	 /* Information about the server */

	WSADATA wsaData;

	/* Open windows connection */
	if (WSAStartup(0x0101, &wsaData) != 0)
	{
		fprintf(stderr, "Could not open Windows connection.\n");
		return(-1);
	}

	/* Clear out server struct */
	memset((void *)&sad, '\0', sizeof(struct sockaddr_in));

	/* Get host name of this computer */
	gethostname(host_name, sizeof(host_name));
	hp = gethostbyname(host_name);

	/* Check for NULL pointer */
	if (hp == NULL)
	{
		fprintf(stderr, "Could not get host name.\n");
		WSACleanup();
		return (-1);
	}

		/* Assign the address */
	sad.sin_addr.S_un.S_un_b.s_b1 = hp->h_addr_list[0][0];
	sad.sin_addr.S_un.S_un_b.s_b2 = hp->h_addr_list[0][1];
	sad.sin_addr.S_un.S_un_b.s_b3 = hp->h_addr_list[0][2];
	sad.sin_addr.S_un.S_un_b.s_b4 = hp->h_addr_list[0][3];

	/* Assign the address */
	sprintf (addr,"%d.%d.%d.%d",sad.sin_addr.S_un.S_un_b.s_b1,
								sad.sin_addr.S_un.S_un_b.s_b2,
								sad.sin_addr.S_un.S_un_b.s_b3,
								sad.sin_addr.S_un.S_un_b.s_b4);

	return 0;
}

int GetIPAddress (char *host, char* addr)
{
	struct  sockaddr_in sad; /* structure to hold server's address  */
	struct	hostent *hp;	 /* Information about the server */

	WSADATA wsaData;

	/* Open windows connection */
	if (WSAStartup(0x0101, &wsaData) != 0)
	{
		fprintf(stderr, "Could not open Windows connection.\n");
		return(-1);
	}

	/* Clear out server struct */
	memset((void *)&sad, '\0', sizeof(struct sockaddr_in));
	hp = gethostbyname(host);

	/* Check for NULL pointer */
	if (hp == NULL)
	{
		fprintf(stderr, "Could not get host name.\n");
		WSACleanup();
		return (-1);
	}

		/* Assign the address */
	sad.sin_addr.S_un.S_un_b.s_b1 = hp->h_addr_list[0][0];
	sad.sin_addr.S_un.S_un_b.s_b2 = hp->h_addr_list[0][1];
	sad.sin_addr.S_un.S_un_b.s_b3 = hp->h_addr_list[0][2];
	sad.sin_addr.S_un.S_un_b.s_b4 = hp->h_addr_list[0][3];

	/* Assign the address */
	sprintf (addr,"%d.%d.%d.%d",sad.sin_addr.S_un.S_un_b.s_b1,
								sad.sin_addr.S_un.S_un_b.s_b2,
								sad.sin_addr.S_un.S_un_b.s_b3,
								sad.sin_addr.S_un.S_un_b.s_b4);

	return 0;
}