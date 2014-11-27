using System;
using System.Net;
using System.Net.Sockets;
using System.IO;
using System.Data;

namespace ZalSystem
{
	/// <summary>
	/// Summary description for Class1.
	/// </summary>
	public class RoboSockets
	{
		public RoboSockets()
		{
			connected = false;
			serverMode = false;
		}

		protected TcpClient myClient;
		protected TcpListener tcpListener;
		protected Socket socketForClient;
															
		private NetworkStream networkStream;
		private StreamReader streamReader;
		private StreamWriter streamWriter;

		//indicates if the socket is used as a server
		protected bool serverMode;
		protected bool connected;

		public bool Connected
		{
			get{ return connected; }
		}

		private int portNo;

		/// <summary>
		/// Port where the connection is being established
		/// </summary>
		public int Port
		{
			get
			{
				if(serverMode)
				{
					socketForClient.RemoteEndPoint.ToString();
					string[] subStrings = socketForClient.RemoteEndPoint.ToString().Split(':');
					return Int16.Parse(subStrings[1]);
				}
				else
				{
					return this.portNo;
				}
			}	
		}

		/// <summary>
		/// Opens a socket which is to be used as server and waits for connection
		/// </summary>
		public void StartServerAt(int port)
		{
			if(!this.Connected)
			{
				TcpListener tcpListener = new TcpListener(IPAddress.Any, port);
				tcpListener.Start();
				//Accepts a new connection...
				socketForClient = tcpListener.AcceptSocket();

				if (socketForClient.Connected)
				{
					//StreamWriter and StreamReader Classes for reading and writing the data to and fro.
					networkStream = new NetworkStream(socketForClient);
					streamWriter = new StreamWriter(networkStream);
					streamReader = new StreamReader(networkStream);
					serverMode = true;
				}
				connected = true;
			}
			else
			{
				throw new Exception("BioSocket already connected");
			}
		}

		/// <summary>
		/// Opens a socket which is going to be used as server
		/// </summary>
		public bool ConnectTo(string serverIP, int port)
		{
			if(!this.Connected)
			{
				//connect to the "localhost" at the give port
				//if you have some other server name then you can use that instead of "localhost"
				try
				{
					//Set server EndPoint to establish the End Point
					IPEndPoint serverPoint = new IPEndPoint(IPAddress.Parse(serverIP), port);

					myClient = new TcpClient(serverIP, port);
				}
				catch
				{
					throw;
				}
				//get a Network stream from the server
				networkStream = myClient.GetStream();
				streamReader = new StreamReader(networkStream);
				streamWriter = new StreamWriter(networkStream);
				connected = true;
				this.portNo = port;
				serverMode = false;
			}
			else
			{
				throw new Exception("BioSocket already connected");
			}
			return true;
		}

		/// <summary>
		/// Writes a string into the socket
		/// </summary>
		public void Write(string message)
		{
			streamWriter.Write(message);
		}

		/// <summary>
		/// Reads the next character in the recieved buffer and moves pointer to next position
		/// </summary>
		public int Read()
		{
			return streamReader.Read();
		}

		/// <summary>
		/// Writes a line into the socket
		/// </summary>
		public void WriteLine(string message)
		{
			streamWriter.WriteLine(message);
			streamWriter.Flush();
		}

		/// <summary>
		/// Reads a line from the socket
		/// </summary>
		public string ReadLine()
		{
			return streamReader.ReadLine();
		}

		/// <summary>
		/// Closes the streams used for socket communication
		/// </summary>
		public void Close()
		{
			streamReader.Close();
			streamWriter.Close();
			networkStream.Close();
			socketForClient.Close();
			tcpListener.Stop();
			connected = false;
		}
	}
}
