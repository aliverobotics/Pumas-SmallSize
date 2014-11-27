/**************************************************
 * File:	SerialPort.cs
 * Description:	Class that handles and configures
 *				the serial port baudrate, byte size,
 *				parity, stop bits, timeout call, 
 *				etc...
 * Author:	Salvador Medina
  *			Lab. Biorobotica
 *			Facultad de Ingenieria, U.N.A.M.
 ***************************************************/


using System;
using System.Data;
using System.IO;
using System.Threading;
using System.Runtime.InteropServices;

namespace ZalSystem
{
	/// <summary>
	/// This class creates an object for the serial port
	/// by means of methods it manages to control the serial port
	/// like: opening, closing, writing and reading
	/// </summary>
	public class SerialPort
	{	
		/// <summary>
		/// Constructor with default values BRate 19200, Size 8, Parity 0, StopBits 1
		/// </summary>
		public SerialPort()
		{
			this.BaudRate	= 9600;
			this.ByteSize	= 8;
			this.Parity		= 0;
			this.StopBits	= 1;
		}

		/// <summary>
		/// Custom constructor for the serial port
		/// </summary>
		/// <param name="BaudRate">Baudrate Setting</param>
		/// <param name="ByteSize">Byte trans size</param>
		/// <param name="Parity">Parity val [0-4] = [no,odd,even,mark,space]</param>
		/// <param name="StopBits">Stop Bits val [0,1,2] = [1, 1.5, 2]</param>
		public SerialPort(int BaudRate, byte ByteSize, byte Parity, byte StopBits)
		{
			this.BaudRate	= BaudRate;
			this.ByteSize	= ByteSize;
			this.Parity		= Parity;
			this.StopBits	= StopBits;
		}

		public int		BaudRate;					//Serial baudrate setting
		public byte		ByteSize;					//Serial byte trans size
		public byte		Parity;						//Serial: 0-4=no,odd,even,mark,space 
		public byte		StopBits;					//Serial: 0,1,2 = 1, 1.5, 2 
		public int		ReadTimeout;				//Serial
		public Thread	serialThread;				//Thread used to read incoming from serial port

		//COMM Port WIN32 File Handle
		public int		hComm	= INVALID_HANDLE_VALUE;		//Default serial handle to "invalid" (WINBASE)
		public bool		Opened	= false;						//Default port monitor to "closed"
	 
		//WIN32 API Constants									//Set win32 cons (including RTS/DTR commands) from WINBASE.h
		private const uint GENERIC_READ			= 0x80000000;			//Needed for "Createfile"
		private const uint GENERIC_WRITE		= 0x40000000;
		private const int OPEN_EXISTING			= 3;		
		private const int INVALID_HANDLE_VALUE	= -1;

		private const int PURGE_TXABORT			= 0x0001;  // Kill the pending/current writes to the comm port.
		private const int PURGE_RXABORT			= 0x0002;  // Kill the pending/current reads to the comm port.
		private const int PURGE_TXCLEAR			= 0x0004;  // Kill the transmit queue if there.
		private const int PURGE_RXCLEAR			= 0x0008;  // Kill the typeahead buffer if there.

		public struct COMMPROP{public uint dwProvSubType;}

		public struct DCB 
		{
			//Serial Definitions Block
			public int DCBlength;								// sizeof(DCB) 
			public int BaudRate;								// Current baud rate
			public uint flags;
			public ushort wReserved;        					// Not currently used 
			public ushort XonLim;           					// Transmit XON threshold 
			public ushort XoffLim;          					// Transmit XOFF threshold 
			public byte ByteSize;								// Number of bits/byte, 4-8 (8)
			public byte Parity;									// 0-4=no,odd,even,mark,space (0)
			public byte StopBits;								// 0,1,2 = 1, 1.5, 2 (0)
			public char XonChar;           						// Tx and Rx XON character 
			public char XoffChar;          						// Tx and Rx XOFF character 
			public char ErrorChar;         						// Error replacement character 
			public char EofChar;           						// End of input character 
			public char EvtChar;            					// Received event character 
			public ushort wReserved1;       					// Reserved; do not use 
		}

		private struct COMMTIMEOUTS
		{
			//Serial Port Timeout Block
			public int ReadIntervalTimeout; 
			public int ReadTotalTimeoutMultiplier; 
			public int ReadTotalTimeoutConstant; 
			public int WriteTotalTimeoutMultiplier; 
			public int WriteTotalTimeoutConstant; 
		} 

		public struct OVERLAPPED 
		{ 
			//Serial Overlapped Block
			public int  Internal; 
			public int  InternalHigh; 
			public int  Offset; 
			public int  OffsetHigh; 
			public int	hEvent; 
		}  

		
		#region IMPORT WIN32 API METHODS
		[DllImport("coredll.dll")]
		private static extern uint GetLastError(
			);
		[DllImport("coredll.dll")]
		private static extern int CreateFile(
			string lpFileName,						// file name
			uint dwDesiredAccess,					// access mode
			uint dwShareMode,						// share mode
			int lpSecurityAttributes,				// SD
			uint dwCreationDisposition,				// how to create
			uint dwFlagsAndAttributes,				// file attributes
			int hTemplateFile						// handle to template file
			);
		[DllImport("coredll.dll")]
		private static extern bool GetCommState(
			int hFile,								// handle to communications device
			ref DCB lpDCB							// device-control block
			);	
		[DllImport("coredll.dll")]
		private static extern bool BuildCommDCB(
			string lpDef,							// device-control string
			ref DCB lpDCB							// device-control block
			);
		[DllImport("coredll.dll")]
		private static extern bool SetCommState(
			int hFile,								// handle to communications device
			ref DCB lpDCB							// device-control block
			);
		[DllImport("coredll.dll")]
		private static extern bool GetCommTimeouts(
			int hFile,								// handle to comm device
			ref COMMTIMEOUTS lpCommTimeouts			// time-out values
			);	
		[DllImport("coredll.dll")]	
		private static extern bool SetCommTimeouts(
			int hFile,								// handle to comm device
			ref COMMTIMEOUTS lpCommTimeouts			// time-out values
			);
		[DllImport("coredll.dll")]
		public static extern bool ReadFile(
			int handle, 
			[In,Out] byte[] buffer, 
			uint bytesToRead, 
			out uint bytesRead, 
			IntPtr overlapped );

		[DllImport("coredll.dll")]	
		private static extern bool WriteFile(
			int hFile,								// handle to file
			byte[] lpBuffer,						// data buffer
			int nNumberOfBytesToWrite,				// number of bytes to write
			ref int lpNumberOfBytesWritten,			// number of bytes written
			ref OVERLAPPED lpOverlapped				// overlapped buffer
			);
		[DllImport("coredll.dll")]
		private static extern bool CloseHandle(
			int hObject								// handle to object
			);
		[DllImport("coredll.dll")]
		private static extern bool SetCommBreak(
			int hFile								// handle to object
			);
		[DllImport("coredll.dll")]
		private static extern bool ClearCommBreak(
			int hFile								// handle to object
			);
		[DllImport("coredll.dll")]
		private static extern bool EscapeCommFunction(
			int hFile,								// handle to object
			byte dwFunc								// pin command
			);
		[DllImport("coredll.dll")]
		private static extern bool PurgeComm(
			int hFile,
			int dwFlags
			);
		#endregion

		//////////////////////////////////////////////////////////////////////////////////////
		//SERIAL METHODS
		//////////////////////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Opens the serial Port
		/// </summary>
		/// <param name="port">Port Name i.e. "COM1:"</param>
		/// <param name="readTimeOutConst">How long is the serial port wait for an answer</param>
		/// <returns></returns>
		public bool Open(string port, int readTimeOutConst)						//port syntax: "COM#:"
		{
			//Configure/Prepare to Open
			Close();										//Close port if already open
			DCB dcbCommPort = new DCB();						//Call DCB struct
			COMMTIMEOUTS ctoCommPort = new COMMTIMEOUTS();		//Call COMMTIMEOUTS struct
			COMMPROP commprop = new COMMPROP();					//Call COMMPROP struct
			//Open the COM port
			hComm = CreateFile (
				port,											//Pointer to the name of the port
				GENERIC_READ | GENERIC_WRITE,					//Access (read-write) mode
				0,												//Share mode
				0,												//Pointer to the security attribute
				OPEN_EXISTING,									//How to open the serial port
				0,												//Port attributes
				0);												//Handle to port with attribute to copy
			// If Port Can't be Opened, Return
			if(this.hComm == INVALID_HANDLE_VALUE) return false;		//Return, update user
			if(this.hComm == INVALID_HANDLE_VALUE) 
			{
				throw(new ApplicationException("Comm Port Can Not Be Opened"));
			}

			// Set the COMM Port Type
			commprop.dwProvSubType = 0x00000001;

			// Set the COMM Timeouts
			GetCommTimeouts(hComm,ref ctoCommPort);
			ctoCommPort.ReadTotalTimeoutConstant = readTimeOutConst;
			ctoCommPort.ReadTotalTimeoutMultiplier = 0;
			ctoCommPort.WriteTotalTimeoutMultiplier = 0;
			ctoCommPort.WriteTotalTimeoutConstant = 0;  
			SetCommTimeouts(hComm,ref ctoCommPort);
	
			// Set BAUD RATE, PARITY, WORD SIZE, AND STOP BITS. (From Above Values)
			GetCommState(hComm, ref dcbCommPort);
			dcbCommPort.BaudRate=BaudRate;
			dcbCommPort.flags=0;
			dcbCommPort.flags|=1;
			if (Parity>0)
			{
				dcbCommPort.flags|=2;
			}
			dcbCommPort.Parity	=Parity;
			dcbCommPort.ByteSize=ByteSize;
			dcbCommPort.StopBits=StopBits;

			if (!SetCommState(hComm, ref dcbCommPort))			//Throw failure exception if needed
			{
				uint ErrorNum = GetLastError();
			}

			Opened = true;										//Port open

			return true;										//Return bool 
		}

		/// <summary>
		/// Opens Serial Port at COM1 with 50 ms of Read Timeout Constant
		/// </summary>
		/// <returns></returns>
		public bool Open()
		{
			return Open("COM1:", 1000);
		}
		
		/// <summary>
		/// Closes the Serial Port
		/// </summary>
		public void Close()									//Close port
		{
			if (hComm != INVALID_HANDLE_VALUE) 
			{
				CloseHandle(hComm);								//Terminate handle
				hComm = INVALID_HANDLE_VALUE;					//Reset handle
			}
			Opened = false;
		}
		
		/// <summary>
		/// Reads one byte from the serial port data buffer
		/// </summary>
		/// <returns></returns>
		public byte Read()									//Read one byte from serial
		{
			uint byteRead;										//Declare buffer
			byte[] BufByte = new byte[1];
			bool result = ReadFile( hComm, BufByte, 1, out byteRead, IntPtr.Zero );	//Read byte from port
			if (result == true)									//If successful, return byte
				return byteRead == 0 ? (byte)0 : BufByte[0];
			else
			{
				return (byte)0;									//Return 0's
			}
		}

		/// <summary>
		/// Reads the serial port buffer
		/// </summary>
		/// <param name="bufferSize">Maximum bytes number to read from buffer</param>
		/// <returns>Bytes read</returns>
		public byte[] ReadBuffer(uint bufferSize)
		{
			uint bytesRead;									//Array to store bytes read
			byte[] bufByte = new byte[bufferSize];
			if(ReadFile( hComm, bufByte, bufferSize, out bytesRead, IntPtr.Zero ))//Read bytes from port
			{
				byte[] returnByte = new byte[bytesRead];	//Here we adjust the array size
				for(int i = 0; i < bytesRead; i++)			//to only return the read bytes
					returnByte[i] = bufByte[i];
				if(bytesRead > 0)
					PurgeComm( hComm, PURGE_TXCLEAR | PURGE_RXCLEAR);//Purge the serial port after read
				return returnByte;
			}
			else
				return new byte[0];							//If nothing read, null =D
		}
		
		/// <summary>
		/// Writes the bytes given into the serial port
		/// </summary>
		/// <param name="WriteBytes">Bytes to be written</param>
		/// <returns>Writing success</returns>
		public bool WriteBytes(byte[] WriteBytes)
		{
			OVERLAPPED ovlCommPort = new OVERLAPPED();
			int BytesWritten = 0;
			return WriteFile(hComm, WriteBytes, WriteBytes.Length, ref BytesWritten, ref ovlCommPort);	//Write byte
		}

		/// <summary>
		/// Writes a string into the serial port
		/// </summary>
		/// <param name="message">String to write</param>
		/// <returns>Writing success</returns>
		public bool WriteString(string message)
		{
			int msgLength = message.Length;
			byte[] sendByte = new byte[msgLength];

			for(int pos = 0; pos < message.Length; pos++)
				sendByte[pos] = Convert.ToByte( (int)message[pos]);
			return WriteBytes( sendByte );
		}

        public bool SendString(string cadena)
        {
             byte[] instruction = new byte[1];

             try
             {
                 instruction[0] = byte.Parse(cadena, System.Globalization.NumberStyles.AllowHexSpecifier);
                 //serialPort1.Write(instruction, 0, 1);
                 return WriteBytes(instruction);
             }
             catch (Exception a)
             {
                 //consoleTextBox.Text +=_strENTER + "error:" + a.Message;
                 return false;
             }
             
        }

	}
}