#include <winsock.h>
/*
typedef struct _WTSoxSysData_{
	char name[10];
	char host[20];
	int  port;
	unsigned char mode;

	SOCKET	sox;					//Descriptor del socket

	//Client Variables
	struct	sockaddr_in addressX;	//Contiene la dirección IP
	short   portX;
	int     longDir;
	char	buffer[SIZESOCK_BUFFER];

	//Server Variables
	struct	hostent *hp;			//Estructura con información sobre el host con el que se va a conectar
	char	hostname[256];			//Nombre del host
};
*/
typedef struct SocketManager{
	SOCKET				soxID;
	struct	sockaddr_in	settings;
} SoxManager;

#define UDP_SERVER 0
#define UDP_CLIENT 1

void WINAPI getSoxError		(char ErrorCode,char *msg);
int  WINAPI CreateUDPSocket	(SoxManager &SocketDescriptor,char *hostname,int port,char socketmode);
int  WINAPI ReadUDPSox		(SoxManager  SocketDescriptor,char *buffer,DWORD BufferSize);
int  WINAPI WriteUDPSox		(SoxManager  SocketDescriptor,char *buffer,DWORD BufferSize);
int  WINAPI KillUDPSocket	(SoxManager  SocketDescriptor);

