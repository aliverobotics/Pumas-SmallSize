using System;
using System.Net;
using System.Net.Sockets;

namespace AsyncServerApp
{
	/// <summary>
	/// Summary description for Class1.
	/// </summary>
	public class ServerASocket
	{
		public  Socket mainSocket;

		private AsyncCallback fncCallBack;

		// An ArrayList is used to keep track of worker sockets that are designed
		// to communicate with each connected client
		public System.Collections.ArrayList clientArrayList = new System.Collections.ArrayList();
		// The following variable will keep track of the cumulative 
		// total number of clients connected at any time

		public delegate void ExternalCallbackFunc(IAsyncResult asyn);
		public int clientCount;

		public ServerASocket()
		{
			clientCount = 0;
		}

		/// <summary>
		/// Gets own machine IP address
		/// </summary>
		/// <returns></returns>
		public string GetMyIP()
		{
			//Get own name
			String strHostName = Dns.GetHostName();
		
			// Find host by name
			IPHostEntry ipHostEntry = Dns.GetHostByName(strHostName);
		
			// Grab the first IP addresses
			String myIP = "";
			foreach(IPAddress ipaddress in ipHostEntry.AddressList)
			{
				myIP = ipaddress.ToString();
				return myIP;
			}
			return myIP;	
		}
		
		/// <summary>
		/// Gets remote IP address from a socket
		/// </summary>
		/// <param name="remoteClient"></param>
		/// <returns></returns>
		public IPAddress GetRemoteIP(System.Net.Sockets.Socket remoteClient)
		{
			return IPAddress.Parse (((IPEndPoint)remoteClient.RemoteEndPoint).Address.ToString ());
		}

		/// <summary>
		/// Starts listening on the port and sets the OnClientConnect handler
		/// </summary>
		/// <param name="port"></param>
		/// <param name="maxClientNum"></param>
		/// <param name="OnClientConnect"></param>
		public void Listen(int port, int maxClientNum, ExternalCallbackFunc OnClientConnect)
		{
			try
			{
				mainSocket = new Socket(AddressFamily.InterNetwork, 
					SocketType.Stream, 
					ProtocolType.Tcp);
				IPEndPoint ipLocal = new IPEndPoint (IPAddress.Any, port);
				// Bind to local IP Address...
				mainSocket.Bind( ipLocal );
				// Start listening...
				mainSocket.Listen(maxClientNum);
				// Create the call back for any client connections...
			
				mainSocket.BeginAccept(new AsyncCallback(OnClientConnect), null);
			}
			catch(SocketException se)
			{
				throw new Exception("Exception caught from Listen()" + se.ToString());
			}
		}		

		/// <summary>
		/// Establishes a wait for data event for a certain socket pack, and set the event handler
		/// </summary>
		/// <param name="socPak"></param>
		/// <param name="OnDataReceived"></param>
		public void WaitForData(SocketPack socPak, ExternalCallbackFunc OnDataReceived)
		{
			try
			{
				if  ( fncCallBack == null )
				{		
					// Specify the call back function which is to be 
					// invoked when there is any write activity by the 
					// connected client
					fncCallBack = new AsyncCallback (OnDataReceived);
				}
	
				socPak.clientSocket.BeginReceive (socPak.dataBuffer, 0, 
					socPak.dataBuffer.Length,
					SocketFlags.None,
					fncCallBack,
					socPak);
			}
			catch(SocketException)
			{
				throw new Exception("I can catch it");
			}
		}

		/// <summary>
		/// Sends a string to the client clientNum in the client list
		/// </summary>
		/// <param name="message"></param>
		/// <param name="clientNum"></param>
		public void SendMsg( string message, int clientNum)
		{
			try
			{
				byte[] byData = System.Text.Encoding.ASCII.GetBytes(message);
				SocketPack socPak = (SocketPack)clientArrayList[clientNum - 1];
				if(socPak.clientSocket != null)
					if(socPak.clientSocket.Connected)
						socPak.clientSocket.Send(byData);		
			}
			catch(SocketException se)
			{
				throw new Exception("Exception caught from SndMsg" + se.ToString());
			}
		}

		/// <summary>
		/// Sends a byte[] to the client clientNum in the client list
		/// </summary>
		/// <param name="message"></param>
		/// <param name="clientNum"></param>
		public void SendMsg( byte[] message, int clientNum)
		{
			try
			{
				SocketPack socPak = (SocketPack)clientArrayList[clientNum - 1];
				if(socPak.clientSocket != null)
					if(socPak.clientSocket.Connected)
						socPak.clientSocket.Send(message);
			}
			catch(SocketException se)
			{
				 throw new Exception(se.ToString());
			}
		}

		/// <summary>
		/// Sends a string to a client with IP Address clientIP (needs to be in the client list)
		/// </summary>
		/// <param name="message"></param>
		/// <param name="clientIP"></param>
		public void SendMsg( string message, string clientIP)
		{
			try{
			byte[] byData = System.Text.Encoding.ASCII.GetBytes(message);
			IPAddress remoteIP = IPAddress.Parse(clientIP);
			foreach(SocketPack socPak in clientArrayList)
			{
				if(socPak.clientIP == remoteIP)
				{
					if(socPak.clientSocket != null)
						if(socPak.clientSocket.Connected)
							socPak.clientSocket.Send(byData);
					break;
				}
			}
			}
			catch(SocketException se)
			{
				throw new Exception("Exception caught from SndMsg" + se.ToString());
			}
		}
		
		/// <summary>
		/// Sends a byte[] to a client with IP Address clientIP (needs to be in the client list)
		/// </summary>
		/// <param name="message"></param>
		/// <param name="clientIP"></param>
		public void SendMsg( byte[] message, string clientIP)
		{
			try
			{
				IPAddress remoteIP = IPAddress.Parse(clientIP);
				foreach(SocketPack socPak in clientArrayList)
				{
					if(socPak.clientIP == remoteIP)
					{
						if(socPak.clientSocket != null)
							if(socPak.clientSocket.Connected)
								socPak.clientSocket.Send(message);
						break;
					}
				}
			}
			catch(SocketException se)
			{
				throw new Exception("Exception caught from SndMsg" + se.ToString());
			}
		}
		
		/// <summary>
		/// Sends a string to all clients in the client list
		/// </summary>
		/// <param name="message"></param>
		public void SendMsg( string message)
		{
			try
			{
				byte[] byData = System.Text.Encoding.ASCII.GetBytes(message);
				foreach(SocketPack socPak in clientArrayList)
				{
					if(socPak.clientSocket != null)
						if(socPak.clientSocket.Connected)
							socPak.clientSocket.Send(byData);					
				}
			}
			catch(SocketException se)
			{
				throw new Exception("Exception caught from SndMsg" + se.ToString());
			}
		}

		/// <summary>
		/// Sends a byte[] to all clients in the client list
		/// </summary>
		/// <param name="message"></param>
		public void SendMsg( byte[] message)
		{
			try
			{
				foreach(SocketPack socPak in clientArrayList)
				{
					if(socPak.clientSocket != null)
						if(socPak.clientSocket.Connected)
							socPak.clientSocket.Send(message);	
				}
			}
			catch(SocketException se)
			{
				throw new Exception("Exception caught from SndMsg" + se.ToString());
			}
		}

		/// <summary>
		/// Disconnects the client clientNum and removes it from the client list
		/// </summary>
		/// <param name="clientNum"></param>
		public void KickClient(int clientNum)
		{
			try
			{
				foreach(SocketPack socPak in clientArrayList)
				{
					if(socPak.clientID == clientNum)
					{
						socPak.clientSocket.Close();
						clientArrayList.Remove(socPak);
						break;
					}
				}
				
			}
			catch(SocketException se)
			{
				throw new Exception("Exception caught from KickClient()" + se.ToString());
			}
		}

		/// <summary>
		/// Disconnects the client with remoteIP address and removes it from the client list
		/// </summary>
		/// <param name="remoteIP"></param>
		public void KickClient(string remoteIP)
		{
			try
			{
				IPAddress targetIP = IPAddress.Parse(remoteIP);
				foreach(SocketPack socPak in clientArrayList)
				{
					if(socPak.clientIP == targetIP)
					{
						socPak.clientSocket.Close();
						clientArrayList.Remove(socPak);
						break;
					}					
				}
			}
			catch(SocketException se)
			{
				throw new Exception("Exception caught from KickClient()" + se.ToString());
			}
		}

		/// <summary>
		/// Closes the server socket and cleans the client list
		/// </summary>
		public void Close()
		{
			try
			{
				if(clientArrayList.Count > 0)
				{
					SocketPack socPak;
					for(int counter = 0; counter < clientArrayList.Count; counter++)
					{
						socPak = (SocketPack)clientArrayList[counter];
						socPak.clientSocket.Close();
						clientArrayList.RemoveAt(counter);
					}
					clientCount = 0;
					mainSocket.Close();
				}
			}
			catch(SocketException se)
			{
				throw new Exception("Exception caught from Close()" + se.ToString());
			}
		}		
	}

	/// <summary>
	/// Contains a socket, client number for the list and the remote IP address
	/// </summary>
	public class SocketPack
	{
		public System.Net.Sockets.Socket clientSocket;
		public int clientID;
		public IPAddress clientIP;

		// Constructor which takes a Socket and a client number
		public SocketPack(System.Net.Sockets.Socket socket, int clientID, IPAddress clientIP)
		{
			this.clientSocket= socket;
			this.clientID    = clientID;
			this.clientIP	 = clientIP; 
		}
		// Buffer to store the data sent by the client
		public byte[] dataBuffer = new byte[1024];
	}
}
