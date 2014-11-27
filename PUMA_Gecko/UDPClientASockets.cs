/********************************************************
 * File:	UDPClientASockets.cs
 * Description:	Class that creates an object to handle
 *				an UDP socket communication with a remote
 *				or local host, it uses events to handle 
 *				the received data in an asynchronous way.
 * Author:	Salvador Medina
 *			Lab. Biorobotica
 *			Facultad de Ingenieria, U.N.A.M.
 *********************************************************/

using System;
using System.Net;
using System.Net.Sockets;

namespace ZalSystem
{
	/// <summary>
	/// This class implements sockets for a client within our robot controlling system.
	/// </summary>
	public class UDPClientASocket
	{
		private Socket sendSocket;				//.NET socket used to connect with our server
		private Socket rcvSocket;
		private EndPoint sendEP;
		private IPEndPoint sendIPEP;
		private EndPoint rcvEP;
		private IPEndPoint rcvIPEP;
		private AsyncCallback fncCallBack ;		//Used to call our asynchronous data reception
		IAsyncResult dataAsyncResult;			//Used to start up our asynchronous data reception
		private SocketPack socPak;				//Our socket package to manipulate socket data within our class
		private System.Text.Encoding encoding;

		public IPAddress ip;

		public bool Connected
		{
			get{ return true;}//sendSocket.Connected; }
		}
		private Counter errCounter;		//Used when a forced connecion occurrs and detect garbage in the socket

		/// <summary>
		/// Data Received Event declaration
		/// When something is received on the socket this event is triggered
		/// </summary>
		public delegate void DataReceivedEventHandler(object sender, SocketPack socData);
		public event DataReceivedEventHandler DataReceived;
		protected virtual void OnDataReceived(SocketPack socData)
		{
			DataReceived(this, socData);
		}

		/// <summary>
		/// Socket Closed Event Declaration
		/// When the socket closes locally or the remote server closes the socket, this event is triggered
		/// </summary>
		public delegate void SocketClosedEventHandler(object sender, SocketPack socData);
		public event SocketClosedEventHandler SocketClosed;
		protected virtual void OnSocketClosed(SocketPack socData)
		{
			SocketClosed(this, socData);
		}

		/// <summary>
		/// Initalize client socket
		/// </summary>
		public UDPClientASocket()
		{
			this.ip = GetMyIP();
			errCounter = new Counter(20); //Number of garbage collected to be determined
			encoding = System.Text.Encoding.UTF8;
		}

		public UDPClientASocket(System.Text.Encoding willEncode)
		{
			this.ip = GetMyIP();
			errCounter = new Counter(20); //Number of garbage collected to be determined
			encoding = willEncode;
		}
		
		/// <summary>
		/// Gets own IP Address
		/// </summary>
		/// <returns>machine's IP address</returns>
		public IPAddress GetMyIP()
		{
			//Get own name
			String strHostName = Dns.GetHostName();
		
			// Find host by name
			IPHostEntry ipHostEntry = Dns.GetHostByName(strHostName);
		
			// Grab the first IP addresses
			IPAddress myIP = null;
			foreach(IPAddress ipaddress in ipHostEntry.AddressList)
			{
				myIP = ipaddress;
				return myIP;
			}
			return myIP;	
		}

		/// <summary>
		/// Gets remote IP address from a socket
		/// </summary>
		/// <param name="remoteClient">socket from which we wanna know IP address</param>
		/// <returns>remote host IP address</returns>
		public IPAddress GetRemoteIP()
		{
			return IPAddress.Parse (((IPEndPoint)sendSocket.RemoteEndPoint).Address.ToString ());
		}

		/// <summary>
		/// Attempts to connect to a remote server
		/// </summary>
		/// <param name="serverIP">server's IP address</param>
		/// <param name="port">Through which port we try to connect</param>
		public void ConnectTo(string serverIP, int sendPort, int rcvPort)
		{
			// Create the socket instance
			sendSocket = new Socket (AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp );
			rcvSocket = new Socket (AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp );

			// Create IPAddress instance
			IPAddress serverIPAddr = IPAddress.Parse(serverIP);

			//Create Endpoints
			this.sendIPEP = new IPEndPoint( serverIPAddr, sendPort );
			this.rcvIPEP = new IPEndPoint(this.GetMyIP(), rcvPort);

			this.sendEP = sendIPEP;
			this.rcvEP = rcvIPEP;

			rcvSocket.Bind(rcvIPEP);
//			sendSocket.Connect(sendIPEP); //Testing purpose
		}

		/// <summary>
		/// Used to wait for data on the socket asynchonously
		/// </summary>
		public void WaitForData()
		{
			if  ( fncCallBack == null ) 
			{
				fncCallBack = new AsyncCallback (WaitForDataCallback);
			}
			socPak = new SocketPack (1024);
			socPak.socket = rcvSocket;
			dataAsyncResult = rcvSocket.BeginReceiveFrom(socPak.dataBuffer, 
													0, 
													socPak.dataBuffer.Length, 
													SocketFlags.None, 
													ref this.rcvEP, 
													fncCallBack, 
													socPak);
		}
		
		/// <summary>
		/// After data received on socket triggers the OnDataReceived Event
		/// </summary>
		/// <param name="asyn">asynchronous answer to call from WaitForData</param>
		SocketPack theSockId;
		private void WaitForDataCallback(IAsyncResult asyn)
		{
			try
			{
				theSockId = (SocketPack)asyn.AsyncState ;		//We catch the asyncronous answer
				int iRx = theSockId.socket.EndReceiveFrom(asyn, ref this.rcvEP);	//And finish it
				if(iRx < 1)			//If less than one, might mean we are receiving "garbage"
				{		//So we start counting for possible errors
					if(this.errCounter.Inc() == errCounter.Maximum)		//If it reaches the maximum possible consecutive errors	
					{
						//Totally shutdown and close the socket and trigger the SocketClosed Event
						theSockId.socket.Shutdown(System.Net.Sockets.SocketShutdown.Both);
						theSockId.socket.Close();
						OnSocketClosed(theSockId);
						this.errCounter.Reset();	//Don't forget to reset error counter 
					}
				}
				else	
				{
					this.errCounter.Reset();						//Reset error counter of all possible garbage received
					SocketPack callBackPack = new SocketPack(iRx);	//repack the data that arrived into a new SocketPack
					callBackPack.socket = theSockId.socket;
					for(int counter=0; counter < iRx; counter++)
					{
						callBackPack.dataBuffer[counter] = theSockId.dataBuffer[counter];
					}
					OnDataReceived(callBackPack);
				}
			}
			catch(System.ObjectDisposedException)
			{	}
			catch(Exception)
			{				
				OnSocketClosed(theSockId);
			}
		}

		/// <summary>
		/// General counter, used in this class to count the possible transmitting errors
		/// </summary>
		public class Counter
		{
			private int initial;	//Initial value of the counter
			private int minimum;	//Minimum possible value for the counter
			private int maximum;	//Maximum possible value for the counter
			private int val;		//Counter value

			public int Minimum		//minimum Interface
			{
				get{return minimum;}
			}

			public int Maximum		//maximum Interface
			{
				get{return maximum;}
			}

			public int Value		//value Interface
			{
				get{return val;}
			}
			
			/// <summary>
			/// Initializes the error counter
			/// Sets the initial val and min at 0
			/// </summary>
			/// <param name="max">counter max value</param>
			public Counter(int max)	//
			{
				initial = 0;
				val = initial;
				minimum = 0;
				maximum = max;
			}

			/// <summary>
			/// Initializes the error Counter
			/// Sets the min at 0
			/// </summary>
			/// <param name="init">counter initial value</param>
			/// <param name="max">counter max value</param>
			public Counter(int init, int max)
			{
				initial = init;
				val = initial;
				minimum = 0;
				maximum = max;
			}

			/// <summary>
			/// Initializes the error Counter
			/// </summary>
			/// <param name="init">counter initial value</param>
			/// <param name="min">counter min value</param>
			/// <param name="max">counter max value</param>
			public Counter(int init, int min, int max)
			{
				initial = init;
				val = initial;
				minimum = min;
				maximum = max;
			}


			/// <summary>
			/// Increments the counter
			/// </summary>
			/// <returns>counter val after inc</returns>
			public int Inc()
			{
				if(++val > maximum)
					return maximum;
				else
					return val;
			}

			/// <summary>
			/// Decreases the counter
			/// </summary>
			/// <returns>counter val after dec</returns>
			public int Dec()
			{
				if(--val < minimum)
					return minimum;
				else
					return val;
			}

			/// <summary>
			/// Resets counter to initial value
			/// </summary>
			public void Reset()
			{
				val = initial;
			}
		}


		/// <summary>
		/// Decodes any byte array into a string
		/// </summary>
		/// <param name="byteArray">raw byte array to be decoded</param>
		/// <param name="encoding">byte array encoding type</param>
		/// <returns>decoded string</returns>
		public string DecodeBytesToString(byte[] byteArray)
		{
			char[] chars = new char[byteArray.Length +  1];
			System.Text.Decoder d = this.encoding.GetDecoder();
			int charLen = d.GetChars(byteArray, 0, byteArray.Length, chars, 0);
			return new System.String(chars);
			
		}

		public byte[] EncodeStringToBytes(string message)
		{
			Byte[] returningBytes = this.encoding.GetBytes(message.ToCharArray());
			return returningBytes;
		}
		
		/// <summary>
		/// Writes a string into the socket
		/// </summary>
		/// <param name="message">message to be sent through the socket</param>
		public void SendMsg(string message)
		{
			byte[] byData = this.EncodeStringToBytes(message);
			sendSocket.SendTo(byData, this.sendEP);
		}

		/// <summary>
		/// Sends a string message with an ENTER at the end
		/// </summary>
		/// <param name="message">message to be sent through the socket</param>
		public void SendLine(string message)
		{
			message += "\r\n";
			SendMsg(message);
		}

		/// <summary>
		/// Obtains substring until the limit string appears
		/// </summary>
		/// <param name="message">string to be processed</param>
		/// <param name="limit">up to which character we want the substring</param>
		/// <returns>processed string</returns>
		public string ReadUntil(string message, string limit)
		{
			return message.Substring(0,message.IndexOf(limit));
		}

		/// <summary>
		/// Closes completely the socket
		/// </summary>
		public void Close()
		{
			if ( (sendSocket != null) || (rcvSocket!=null))	//Check if socket still exists
			{
				rcvSocket.EndReceiveFrom(dataAsyncResult, ref this.rcvEP);
				rcvSocket.Shutdown(System.Net.Sockets.SocketShutdown.Both);
				rcvSocket.Close();
				sendSocket.Shutdown(System.Net.Sockets.SocketShutdown.Both);
				sendSocket.Close();
			}
		}
	}

	/// <summary>
	/// This class wraps up our socket data
	/// </summary>
	public class SocketPack : EventArgs
	{
		/// <summary>
		/// Initializes the socketPack
		/// </summary>
		/// <param name="bufferSize">Size of reading buffer allowed</param>
		public SocketPack(int bufferSize)
		{
			dataBuffer = new byte[bufferSize];
		}
		public System.Net.Sockets.Socket socket;	//Socket
		public byte[] dataBuffer;					//What was last read from the dataBuffer
	}
}