using System;
using System.Drawing;
using System.Collections;
using System.Windows.Forms;
using System.Data;
using System.Net;
using System.Threading;
using ZalSystem;


namespace Gecko
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		private System.Windows.Forms.TextBox serverIPTextBox;
		private System.Windows.Forms.TextBox consoleTextBox;		
		private System.Windows.Forms.MainMenu mainMenu1;
		private System.Windows.Forms.Button startButton;
		private System.Windows.Forms.Label label2;
		
		private ZalRobot mrRoboto;			//Robot instance
		
		private UDPClientASocket clientSoc;	//Socket
		private ExecList mainCmdRequest;
		private static bool executing;
		private int commandCount;

		#region Constants
		private char	_charSPACE = ' ';
		private string	_strENTER = "\r\n";
		private char    _charNULL  = '\0';
		private System.Windows.Forms.TextBox sendPortTextBox;
		private System.Windows.Forms.Label sendPortLabel;
		private System.Windows.Forms.TextBox rcvPortTextBox;
		private System.Windows.Forms.Label rcvPortLabel;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Label countLabel;
		private System.Windows.Forms.Button clearButton;
		private int		_intERROR = -1;
		private System.Windows.Forms.Label localIPLabel;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.PictureBox leftIDPicBox;
		private System.Windows.Forms.PictureBox rightIDPicBox;
		
		private string[][] IPPortChart = {new string[]{"127.0.0.1"},
										  new string[]{"111", "8001", "8002", "ggp"},
										  new string[]{"112", "8003", "8004", "ggb"},
										  new string[]{"113", "8005", "8006", "gpb"},
										  new string[]{"114", "8007", "8008", "ppg"},
										  new string[]{"115", "8009", "8010", "ppb"}};
		#endregion

		public Form1()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			this.mrRoboto				= new ZalRobot();
			this.clientSoc				= new UDPClientASocket(System.Text.Encoding.ASCII);
			
			InitIPPortGUI();

			this.clientSoc.DataReceived	+= new ZalSystem.UDPClientASocket.DataReceivedEventHandler(clientSoc_DataReceived);
			this.clientSoc.SocketClosed	+= new ZalSystem.UDPClientASocket.SocketClosedEventHandler(clientSoc_SocketClosed); 
			
			this.KeyDown += new KeyEventHandler(Form1_KeyDown);

			commandCount = 0;
			mainCmdRequest = new ExecList();
			executing = false;

		}
		
		void InitIPPortGUI()
		{
			try
			{
				string myIP = clientSoc.GetMyIP().ToString();
				string[] temp = myIP.Split('.');
				string localNum = temp[temp.Length-1];
				this.localIPLabel.Text =  myIP;
				for(int n=0; n<IPPortChart.Length; n++)
				{
					if(localNum == IPPortChart[n][0])
					{
						if(n==0)
						{
							MessageBox.Show("No esta conectado a una red\r\n Se cerrara el programa");
							this.Close();
						}
						this.rcvPortTextBox.Text = IPPortChart[n][1];
						this.sendPortTextBox.Text = IPPortChart[n][2];
						break;
					}
				}
			}
			catch(Exception){
			}
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		///
		protected override void Dispose(bool disposing)
		{
			mrRoboto.sPort.Close();
			clientSoc.Close();
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			System.Resources.ResourceManager resources = new System.Resources.ResourceManager(typeof(Form1));
			this.mainMenu1 = new System.Windows.Forms.MainMenu();
			this.sendPortTextBox = new System.Windows.Forms.TextBox();
			this.sendPortLabel = new System.Windows.Forms.Label();
			this.startButton = new System.Windows.Forms.Button();
			this.consoleTextBox = new System.Windows.Forms.TextBox();
			this.localIPLabel = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.serverIPTextBox = new System.Windows.Forms.TextBox();
			this.rcvPortTextBox = new System.Windows.Forms.TextBox();
			this.rcvPortLabel = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.countLabel = new System.Windows.Forms.Label();
			this.clearButton = new System.Windows.Forms.Button();
			this.label1 = new System.Windows.Forms.Label();
			this.leftIDPicBox = new System.Windows.Forms.PictureBox();
			this.rightIDPicBox = new System.Windows.Forms.PictureBox();
			// 
			// sendPortTextBox
			// 
			this.sendPortTextBox.Location = new System.Drawing.Point(72, 48);
			this.sendPortTextBox.Size = new System.Drawing.Size(40, 20);
			this.sendPortTextBox.Text = "8000";
			// 
			// sendPortLabel
			// 
			this.sendPortLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 6F, System.Drawing.FontStyle.Bold);
			this.sendPortLabel.ForeColor = System.Drawing.Color.DarkGray;
			this.sendPortLabel.Location = new System.Drawing.Point(16, 56);
			this.sendPortLabel.Size = new System.Drawing.Size(48, 8);
			this.sendPortLabel.Text = "Send Port:";
			// 
			// startButton
			// 
			this.startButton.Location = new System.Drawing.Point(184, 16);
			this.startButton.Size = new System.Drawing.Size(40, 24);
			this.startButton.Text = "Start";
			this.startButton.Click += new System.EventHandler(this.startButton_Click);
			// 
			// consoleTextBox
			// 
			this.consoleTextBox.Location = new System.Drawing.Point(0, 96);
			this.consoleTextBox.Multiline = true;
			this.consoleTextBox.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
			this.consoleTextBox.Size = new System.Drawing.Size(240, 144);
			this.consoleTextBox.Text = "";
			// 
			// localIPLabel
			// 
			this.localIPLabel.Font = new System.Drawing.Font("Impact", 9.75F, System.Drawing.FontStyle.Regular);
			this.localIPLabel.ForeColor = System.Drawing.Color.DarkBlue;
			this.localIPLabel.Location = new System.Drawing.Point(72, 0);
			this.localIPLabel.Size = new System.Drawing.Size(104, 16);
			// 
			// label2
			// 
			this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold);
			this.label2.ForeColor = System.Drawing.Color.DarkGray;
			this.label2.Location = new System.Drawing.Point(16, 24);
			this.label2.Size = new System.Drawing.Size(64, 16);
			this.label2.Text = "Remote IP:";
			// 
			// serverIPTextBox
			// 
			this.serverIPTextBox.Location = new System.Drawing.Point(88, 24);
			this.serverIPTextBox.Size = new System.Drawing.Size(88, 20);
			this.serverIPTextBox.Text = "192.168.169.";
			// 
			// rcvPortTextBox
			// 
			this.rcvPortTextBox.Location = new System.Drawing.Point(184, 48);
			this.rcvPortTextBox.Size = new System.Drawing.Size(40, 20);
			this.rcvPortTextBox.Text = "8001";
			// 
			// rcvPortLabel
			// 
			this.rcvPortLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 6F, System.Drawing.FontStyle.Bold);
			this.rcvPortLabel.ForeColor = System.Drawing.Color.DarkGray;
			this.rcvPortLabel.Location = new System.Drawing.Point(128, 56);
			this.rcvPortLabel.Size = new System.Drawing.Size(48, 8);
			this.rcvPortLabel.Text = "Rcv Port:";
			// 
			// label3
			// 
			this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 6F, System.Drawing.FontStyle.Bold);
			this.label3.ForeColor = System.Drawing.Color.DarkGray;
			this.label3.Location = new System.Drawing.Point(16, 80);
			this.label3.Size = new System.Drawing.Size(48, 8);
			this.label3.Text = "Commands:";
			// 
			// countLabel
			// 
			this.countLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 6F, System.Drawing.FontStyle.Bold);
			this.countLabel.ForeColor = System.Drawing.Color.DarkGray;
			this.countLabel.Location = new System.Drawing.Point(72, 80);
			this.countLabel.Size = new System.Drawing.Size(48, 8);
			// 
			// clearButton
			// 
			this.clearButton.Location = new System.Drawing.Point(128, 72);
			this.clearButton.Size = new System.Drawing.Size(48, 20);
			this.clearButton.Text = "Clear";
			this.clearButton.Click += new System.EventHandler(this.clearButton_Click);
			// 
			// label1
			// 
			this.label1.Font = new System.Drawing.Font("Impact", 8.25F, System.Drawing.FontStyle.Regular);
			this.label1.ForeColor = System.Drawing.Color.DarkBlue;
			this.label1.Location = new System.Drawing.Point(24, 0);
			this.label1.Size = new System.Drawing.Size(40, 16);
			this.label1.Text = "Local IP:";
			// 
			// leftIDPicBox
			// 
			this.leftIDPicBox.Location = new System.Drawing.Point(80, 216);
			this.leftIDPicBox.Size = new System.Drawing.Size(32, 32);
			// 
			// rightIDPicBox
			// 
			this.rightIDPicBox.Location = new System.Drawing.Point(128, 216);
			this.rightIDPicBox.Size = new System.Drawing.Size(32, 32);
			// 
			// Form1
			// 
			this.BackColor = System.Drawing.Color.Red;
			this.Controls.Add(this.label1);
			this.Controls.Add(this.clearButton);
			this.Controls.Add(this.localIPLabel);
			this.Controls.Add(this.consoleTextBox);
			this.Controls.Add(this.startButton);
			this.Controls.Add(this.sendPortLabel);
			this.Controls.Add(this.sendPortTextBox);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.serverIPTextBox);
			this.Controls.Add(this.rcvPortTextBox);
			this.Controls.Add(this.rcvPortLabel);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.countLabel);
			this.Controls.Add(this.leftIDPicBox);
			this.Controls.Add(this.rightIDPicBox);
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MaximizeBox = false;
			this.Menu = this.mainMenu1;
			this.MinimizeBox = false;
			this.Text = "Robocup UDP";

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>

		static void Main() 
		{
			Application.Run(new Form1());
		}

		private void startButton_Click(object sender, System.EventArgs e)
		{
			IgniteTerminal();
		}

		private void IgniteTerminal()
		{
			mrRoboto.sPort.Open();
			startButton.Enabled = false;
			consoleTextBox.Text += "Waiting for connection..." + _strENTER;

			try
			{
				clientSoc.ConnectTo(serverIPTextBox.Text, Int32.Parse(sendPortTextBox.Text), Int32.Parse(rcvPortTextBox.Text));
				consoleTextBox.Text += "Connected to server @"
					+ ":" + sendPortTextBox.Text + _strENTER;
				if(clientSoc.Connected)
				{
					consoleTextBox.Text += "Client connected\r\n";
					clientSoc.WaitForData();
				}
			}
			catch(Exception e234)
			{
				MessageBox.Show("Error while trying to connect:"+ _strENTER + e234.ToString());
				consoleTextBox.Text = e234.ToString();
				clientSoc.Close();
				startButton.Enabled = true;
			}
		}

		private void clientSoc_SocketClosed(object sender, SocketPack socPak)
		{
			consoleTextBox.Text += _strENTER + "Close Event occurred";
			clientSoc.Close();
			startButton.Enabled = true;
			consoleTextBox.Text = "";
		}
		

		string szData;
		private void clientSoc_DataReceived(object sender, SocketPack socPak)
		{
			try
			{
				szData = clientSoc.DecodeBytesToString(socPak.dataBuffer);
				this.consoleTextBox.Text += szData + "\r\n";
				ExecList cmdRequest = ParseData(szData);
				if(!executing)
				{
					executing = true;
					mainCmdRequest = cmdRequest;
					Thread actionThread = new Thread( new System.Threading.ThreadStart(Execute));
					actionThread.Start();
				}
				clientSoc.WaitForData();
			}
			catch(System.NullReferenceException sne)
			{
				consoleTextBox.Text += "Error @ OnDataReceived NULL ref";
			}
			catch(System.Net.Sockets.SocketException)
			{
				consoleTextBox.Text = "Disconnected";
				clientSoc.Close();
			}
			catch (Exception ex)
			{
				consoleTextBox.Text += "Error @ OnDataReceived" + ex.ToString();
			}
		}

		//------------------------------------------------------------------------------------
		//Function: Parse
		//Purpose:  Parses the incoming string from sockets and executes an action
		//Accepts:	Message incoming from sockets
		//Returns:	Whether the string is acceptable in the established grammar
		//Throws:	
		//------------------------------------------------------------------------------------
		/// <summary>
		/// Parses a command string, and executes de command
		/// </summary>
		/// <param name="inMessage">string to be parsed</param>
		/// <returns>Parse success</returns>
		
		private string[] msgTokens;
		public ExecList ParseData(string inMessage)
		{
			ExecList cmdRequest = new ExecList();

			if(inMessage == null)
				return cmdRequest;

			//First, we clean the message from those characters that we found useless
			inMessage = clientSoc.ReadUntil(inMessage, '\n'.ToString());
			inMessage = inMessage.TrimEnd(new char[]{_charSPACE, _charNULL});
	
			//Second, we look for a command ID or the sender's IP
			if( inMessage.IndexOf("@") != _intERROR)
			{			
				msgTokens = inMessage.Split('@');
				
				try
				{
					cmdRequest.cmdID = int.Parse(msgTokens[1]);
					inMessage = msgTokens[0];
				}
				catch(System.FormatException)
				{
					try
					{
						if( msgTokens[1].Equals(clientSoc.ip.ToString()) )
						{
							inMessage = msgTokens[0];
						}
					}
					catch(System.FormatException){
						cmdRequest = new ExecList();
						return cmdRequest;
					}
				}
			}	

			//Clean the command submessage and divide in commandTokens
			inMessage = inMessage.ToLower();
			string[] commandTokens;
			inMessage = inMessage.TrimEnd(_charSPACE);
			commandTokens = inMessage.Split(_charSPACE);	//Our grammar divides commandTokens with space character
			
			int spaceCont = 0;
			for(int tokCont=0; tokCont < commandTokens.Length; tokCont++ )
			{
				if(commandTokens[tokCont]=="")
					spaceCont++;
			}

			string[] finalTokens = new string[commandTokens.Length-spaceCont];
			int finalCont = 0;
			for(int tokCont = 0; tokCont < commandTokens.Length; tokCont++)
			{
				if(commandTokens[tokCont] != ""){
					finalTokens[finalCont] = commandTokens[tokCont];
					finalCont++;
				}
			}

			try
			{
				switch(finalTokens[0])
				{
					//To Be Confirmed  commands (depends whether they are programmed on robot or not)
					#region TBC Commands
					//					case "hp":
					//						break;
					//					case "left":
					//						mrRoboto.TurnLeft();
					//						consoleTextBox.Text += "\r\nI'm here";
					//						break;
					//					case "right":
					//						mrRoboto.TurnRight();
					//						break;
					//					case "forward":
					//						mrRoboto.MoveForward();
					//						break;
					//					case "backward":
					//						mrRoboto.MoveBackward();
					//						break;
					//					case "stop":
					//						mrRoboto.Stop();
					//						break;
					//					case "raw":
					//						if(commandTokens.Length > 1)
					//						{
					//							mrRoboto.sPort.WriteString(commandTokens[1]);
					//							lastMessage = commandTokens[1];
					//							messageSPort = commandTokens[1];
					//						}
					//						break;
					//					case "setid":
					//						if(commandTokens.Length>1)
					//						{
					//							mrRoboto.idNum = Int32.Parse(commandTokens[1]); 
					//						}
					//						break;
					//					case "setsp":
					//						if(commandTokens.Length > 1)
					//						{
					//							mrRoboto.SetSpeed(Int32.Parse(commandTokens[1]));
					//						}
					//						break;
					//					case "shid":
					//						if(clientSoc.clientSocket.Connected)
					//						{
					//							clientSoc.SendMsg(mrRoboto.idNum.ToString());
					//						}
					//						break;
					//					case "shs":
					//						if(commandTokens.Length>1)
					//						{
					//							clientSoc.SendMsg(mrRoboto.ReqSensorVal(commandTokens[1], Int32.Parse(commandTokens[2])));
					//						}
					//						break;
					//					case "bye":
					//						throw new Exception("Call to end program");
					#endregion
					case "mv":	
						if(finalTokens.Length > 2)
						{
							try
							{	
								cmdRequest.mvReq = true;
								cmdRequest.mvDist = float.Parse(finalTokens[1], System.Globalization.NumberStyles.Float);
								cmdRequest.mvAngle = float.Parse(finalTokens[2], System.Globalization.NumberStyles.Float);
							}
							catch(System.Exception se)
							{
							}
						}
						break;
					case "raw":	//Sends directly 1st parameter to serial port
						if(finalTokens[1].Length == 10)
							mrRoboto.sPort.WriteString(finalTokens[1]);
						this.consoleTextBox.Text += finalTokens[1] + "\r\n";
						break;
					case "shooton":
					{
						mrRoboto.ShootOn();
					}
						break;
					case "shootoff":
					{
						mrRoboto.ShootOff();
					}
						break;
					default: break;
				}

			}
			catch(Exception excp)
			{
				consoleTextBox.Text += _strENTER + "Error while Parsing."
					+ _strENTER + excp.ToString();
				cmdRequest = new ExecList();
			}
			return cmdRequest;
		}

		/// <summary>
		/// When the ENTER button is pressed on the ppc, 
		/// Application tries to connect to server
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void Form1_KeyDown(object sender, KeyEventArgs e)
		{
			if(e.KeyCode == Keys.Return && startButton.Enabled == true)
				IgniteTerminal();
		}


		/// <summary>
		/// Executes the execute command list requested in mainCmdRequest
		/// </summary>
		private void Execute()
		{
			consoleTextBox.Text = "Command #" + this.commandCount.ToString();
			try{
				if(mainCmdRequest.shOnReq)
				{
					consoleTextBox.Text += _strENTER + "Activating Shooter...";
					mrRoboto.ShootOn();
					mainCmdRequest.shOnReq = false;
					Thread.Sleep(50);
				}
				if(mainCmdRequest.shOffReq)
				{
					consoleTextBox.Text += _strENTER + "Deactivating Shooter...";
					mrRoboto.ShootOff();
					mainCmdRequest.shOffReq = false;
					Thread.Sleep(50);
				}
				if(mainCmdRequest.mvReq)
				{
					consoleTextBox.Text += _strENTER + "Moving Robot";
					mrRoboto.TurnAndMove(mainCmdRequest.mvDist, mainCmdRequest.mvAngle);
					clientSoc.SendMsg(mainCmdRequest.mvAngle.ToString() + " " + mainCmdRequest.mvDist.ToString() + " ");
					mainCmdRequest.mvReq = false;
					Thread.Sleep(50);
				}
			}
			catch(System.ArgumentOutOfRangeException)
			{
				consoleTextBox.Text += _strENTER + "**ERROR: Sync Lost, Reset Robot";
				clientSoc.SendMsg("0 0 ");
			}
			commandCount++;
			//countLabel.Text = commandCount.ToString();
			executing = false;
		}

		private void clearButton_Click(object sender, System.EventArgs e)
		{
			countLabel.Text = commandCount.ToString();
		}

	}

	public struct ExecList
	{
		public int		cmdID;
		public bool		mvReq;
		public bool		shOnReq;
		public bool		shOffReq;
		public float	mvDist;
		public float	mvAngle;
	}
}