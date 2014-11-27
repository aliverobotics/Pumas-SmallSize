//opens a server
long openchn()
{
	int inport,outport;
	char *host;
	DATA_OBJECT temp;

	/* Check for exactly 2 argument. */
	if (ArgCountCheck("openchn",EXACTLY,3) == -1)
	{ return(1L); }
	host=RtnLexeme(1);

	if (ArgTypeCheck("openchn",2,INTEGER,&temp) == 0)
	{ return(1L); }
	{ inport = (int)DOToLong(temp); }

	if (ArgTypeCheck("MysockOn",3,INTEGER,&temp) == 0)
	{ return(1L); }
	{ outport = (int)DOToLong(temp); }

	//opens the server
	return Comm.Add_Channel("",host,inport,outport);
}

//closes socket
long closechn()
{
	int  channelnum;
	DATA_OBJECT temp;

	/* Check for exactly 4 arguments. */
	if (ArgCountCheck("closechn",EXACTLY,1) == -1)
	{ return(1L); }
	if (ArgTypeCheck("closechn",1,INTEGER,&temp) == 0)
	{ return(1L); }

	{ channelnum = (int)DOToLong(temp); }

	//closes the socket
	Comm.Close_Channel(channelnum);

	return 0;
}

//sends a string
long chnsend()
{
	char * message;
	int	   socknum;
	DATA_OBJECT temp;

	/* Check for exactly 2 argument. */
	if (ArgCountCheck("chnsend",EXACTLY,2) == -1)
	{ return(1L); }
	
	if (ArgTypeCheck("chnsend",1,INTEGER,&temp) == 0)
	{ return(1L); }

	{ socknum = (int)DOToLong(temp); }

	message=RtnLexeme(2);
	sprintf (message, "%s\n", message);
	printf ("sending %d => %s",socknum,message);
	return (Comm.Channel_Send(socknum,message));
}

//recives a string
char * chnrecv()
{
	int	   socknum;
	DATA_OBJECT temp;

	/* Check for exactly 1 argument. */
	if (ArgCountCheck("chnrecv",EXACTLY,1) == -1)
	{ return(NULL); }
	
	if (ArgTypeCheck("chnrecv",1,INTEGER,&temp) == 0)
	{ return(NULL); }

	{ socknum = (int)DOToLong(temp); }
	return (Comm.Channel_Receive(socknum));
}