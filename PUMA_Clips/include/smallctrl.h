int socks[5]; //channels for communication with robots

//opens communications with trainer
void init_sockets(char* host, int baseport)
{
	int i;
	for (i=0;i<5;i++)
		socks[i] = Comm.Add_Channel ( "", host, 
									 baseport + 2001 + i*100 + 1, 
									 baseport + 2001 + i*100);

	for (i=0;i<5;i++) 
		Comm.Open_Channel(socks[i]);
}

//opens communications
void close_sockets(void)
{
	int i;
	for (i=0;i<5;i++)
		Comm.Close_Channel(socks[i]);
}

long sendrobot()
{
	char * message;
	char string[80];
	int	   robotnum;
	DATA_OBJECT temp;

	/* Check for exactly 2 argument. */
	if (ArgCountCheck("sendrobot",EXACTLY,2) == -1)
	{ return(1L); }
	
	if (ArgTypeCheck("sendrobot",1,INTEGER,&temp) == 0)
	{ return(1L); }

	{ robotnum = (int)DOToLong(temp); }

	message=RtnLexeme(2);
	sprintf (string, "%s\n", message);
	printf ("sending %d => %s",robotnum,string);
	return (Comm.Channel_Send( socks[robotnum], string ));
}

long sendall()
{
	char * message;
	char string[80];
	int	   robotnum;

	/* Check for exactly 2 argument. */
	if (ArgCountCheck("sendrobot",EXACTLY,1) == -1)
	{ return(1L); }

	message=RtnLexeme(1);
	sprintf (string, "%s\n", message);
	printf ("sending => %s",string);
	for (robotnum=0;robotnum<4;robotnum++)
	     Comm.Channel_Send( socks[robotnum], string );

	return 1;
}
