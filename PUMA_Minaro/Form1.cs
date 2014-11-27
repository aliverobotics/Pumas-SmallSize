using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using Microsoft.DirectX.DirectInput;
using ZalSystem;
using System.Threading;

namespace Minaro
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		private System.ComponentModel.IContainer components;
		private System.Windows.Forms.Label dummy2;

		private System.Windows.Forms.Timer timer1;
		internal ZalJoystick myJStick;
		internal ZalSystem.UDPClientASocket socket;

		private int[] posXY;
		private int POV;
		private int gamepadThold;
		private int axisCount;
		private int[] axisFRSign;
		private string trainButton;
		private const int JOYSTICK_MAX = 661;
		private const int MAX_SPEED = 512;
		private double rad45;
		private string remoteIPString;
		private int sendPort;
		private int rcvPort;
		private string buttonString;
		private string posString;
		private string lastLeftCommand;
		private string lastRightCommand;
		private string lastShootCommand;
		private string lastTrainCommand;
		
		private ZalCircle guiCircle;
		private ZalCircle limitCircle;
		private ZColorProgressBar.ZColorProgressBar leftUpBar;
		private ZColorProgressBar.ZColorProgressBar leftDownBar;
		private ZColorProgressBar.ZColorProgressBar rightDownBar;
		private ZColorProgressBar.ZColorProgressBar rightUpBar;

		private System.Windows.Forms.MainMenu mainMenu1;
		private System.Windows.Forms.MenuItem menuItem1;
		private System.Windows.Forms.MenuItem menuItem2;
		private System.Windows.Forms.MenuItem menuItem3;
		private System.Windows.Forms.MenuItem menuItem4;
		double x;
		private System.Windows.Forms.MenuItem menuItem5;
		private System.Windows.Forms.MenuItem menuItem6;
		private System.Windows.Forms.MenuItem menuItem9;
		private System.Windows.Forms.MenuItem menuItem10;
		private System.Windows.Forms.MenuItem menuItem7;
		double y;


		protected override void OnPaint(PaintEventArgs e)
		{
			// Handles
			Graphics stringButtons = e.Graphics;
			Graphics circles = e.Graphics;
			Graphics graphics = this.CreateGraphics();

			// Pens
			Pen yellowPen = new Pen(Color.Yellow, 3);

			// Points
			PointF buttonStringPos = new PointF(10,200);

			// Brushes
			Brush blackBrush = new SolidBrush(Color.Black);
			Brush limeBrush = new SolidBrush(Color.Lime);

			// Font Info
			FontFamily familyName = new FontFamily("Impact");
			Font impactFont = new Font(familyName, 24, FontStyle.Regular, GraphicsUnit.Pixel);

			// Form
			stringButtons.DrawString(buttonString, impactFont, limeBrush, buttonStringPos);
			circles.DrawEllipse( yellowPen, guiCircle.CenterX-guiCircle.Radius, guiCircle.CenterY-guiCircle.Radius, guiCircle.Radius*2, guiCircle.Radius*2);	
		}

		public Form1()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();
			
			myJStick = new ZalJoystick(this, JOYSTICK_MAX);
			myJStick.InitDirectInput();
			this.gamepadThold = 50;
			this.axisCount = 0;
			this.axisFRSign = new int[4];
			this.socket = new UDPClientASocket();

			this.x = this.Width/2;
			this.y = this.Height/2;
			this.guiCircle = new ZalCircle(this.ClientSize.Width/2, this.ClientSize.Height/2, this.ClientSize.Width/2);
			this.limitCircle = new ZalCircle(0, 0, JOYSTICK_MAX);
			rad45 = (Math.PI/180) * 45;
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				myJStick.Unacquire();

				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.components = new System.ComponentModel.Container();
			this.timer1 = new System.Windows.Forms.Timer(this.components);
			this.dummy2 = new System.Windows.Forms.Label();
			this.leftUpBar = new ZColorProgressBar.ZColorProgressBar();
			this.leftDownBar = new ZColorProgressBar.ZColorProgressBar();
			this.rightDownBar = new ZColorProgressBar.ZColorProgressBar();
			this.rightUpBar = new ZColorProgressBar.ZColorProgressBar();
			this.mainMenu1 = new System.Windows.Forms.MainMenu();
			this.menuItem1 = new System.Windows.Forms.MenuItem();
			this.menuItem2 = new System.Windows.Forms.MenuItem();
			this.menuItem3 = new System.Windows.Forms.MenuItem();
			this.menuItem4 = new System.Windows.Forms.MenuItem();
			this.menuItem5 = new System.Windows.Forms.MenuItem();
			this.menuItem7 = new System.Windows.Forms.MenuItem();
			this.menuItem6 = new System.Windows.Forms.MenuItem();
			this.menuItem9 = new System.Windows.Forms.MenuItem();
			this.menuItem10 = new System.Windows.Forms.MenuItem();
			this.SuspendLayout();
			// 
			// timer1
			// 
			this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
			// 
			// dummy2
			// 
			this.dummy2.Anchor = System.Windows.Forms.AnchorStyles.None;
			this.dummy2.BackColor = System.Drawing.Color.Aquamarine;
			this.dummy2.ForeColor = System.Drawing.Color.DarkTurquoise;
			this.dummy2.Location = new System.Drawing.Point(150, 150);
			this.dummy2.Name = "dummy2";
			this.dummy2.Size = new System.Drawing.Size(150, 150);
			this.dummy2.TabIndex = 0;
			this.dummy2.Text = "AKI TOI!";
			// 
			// leftUpBar
			// 
			this.leftUpBar.BarColor = System.Drawing.Color.Lime;
			this.leftUpBar.BGColor = System.Drawing.Color.FromArgb(((System.Byte)(0)), ((System.Byte)(64)), ((System.Byte)(0)));
			this.leftUpBar.BorderColor = System.Drawing.Color.Black;
			this.leftUpBar.FillStyle = ZColorProgressBar.ZColorProgressBar.FillStyles.Solid;
			this.leftUpBar.Location = new System.Drawing.Point(80, 40);
			this.leftUpBar.Maximum = 513;
			this.leftUpBar.Minimum = 0;
			this.leftUpBar.Name = "leftUpBar";
			this.leftUpBar.Orientation = ZColorProgressBar.ZColorProgressBar.Orientations.Upwards;
			this.leftUpBar.Size = new System.Drawing.Size(56, 120);
			this.leftUpBar.Step = 1;
			this.leftUpBar.TabIndex = 2;
			this.leftUpBar.Value = 0;
			// 
			// leftDownBar
			// 
			this.leftDownBar.BarColor = System.Drawing.Color.Red;
			this.leftDownBar.BGColor = System.Drawing.Color.FromArgb(((System.Byte)(64)), ((System.Byte)(0)), ((System.Byte)(0)));
			this.leftDownBar.BorderColor = System.Drawing.Color.Black;
			this.leftDownBar.FillStyle = ZColorProgressBar.ZColorProgressBar.FillStyles.Solid;
			this.leftDownBar.Location = new System.Drawing.Point(80, 160);
			this.leftDownBar.Maximum = 513;
			this.leftDownBar.Minimum = 0;
			this.leftDownBar.Name = "leftDownBar";
			this.leftDownBar.Orientation = ZColorProgressBar.ZColorProgressBar.Orientations.Downwards;
			this.leftDownBar.Size = new System.Drawing.Size(56, 120);
			this.leftDownBar.Step = 1;
			this.leftDownBar.TabIndex = 2;
			this.leftDownBar.Value = 0;
			// 
			// rightDownBar
			// 
			this.rightDownBar.BarColor = System.Drawing.Color.Red;
			this.rightDownBar.BGColor = System.Drawing.Color.FromArgb(((System.Byte)(64)), ((System.Byte)(0)), ((System.Byte)(0)));
			this.rightDownBar.BorderColor = System.Drawing.Color.Black;
			this.rightDownBar.FillStyle = ZColorProgressBar.ZColorProgressBar.FillStyles.Solid;
			this.rightDownBar.Location = new System.Drawing.Point(192, 160);
			this.rightDownBar.Maximum = 513;
			this.rightDownBar.Minimum = 0;
			this.rightDownBar.Name = "rightDownBar";
			this.rightDownBar.Orientation = ZColorProgressBar.ZColorProgressBar.Orientations.Downwards;
			this.rightDownBar.Size = new System.Drawing.Size(56, 120);
			this.rightDownBar.Step = 1;
			this.rightDownBar.TabIndex = 2;
			this.rightDownBar.Value = 0;
			// 
			// rightUpBar
			// 
			this.rightUpBar.BarColor = System.Drawing.Color.Lime;
			this.rightUpBar.BGColor = System.Drawing.Color.FromArgb(((System.Byte)(0)), ((System.Byte)(64)), ((System.Byte)(0)));
			this.rightUpBar.BorderColor = System.Drawing.Color.Black;
			this.rightUpBar.FillStyle = ZColorProgressBar.ZColorProgressBar.FillStyles.Solid;
			this.rightUpBar.Location = new System.Drawing.Point(192, 40);
			this.rightUpBar.Maximum = 513;
			this.rightUpBar.Minimum = 0;
			this.rightUpBar.Name = "rightUpBar";
			this.rightUpBar.Orientation = ZColorProgressBar.ZColorProgressBar.Orientations.Upwards;
			this.rightUpBar.Size = new System.Drawing.Size(56, 120);
			this.rightUpBar.Step = 1;
			this.rightUpBar.TabIndex = 3;
			this.rightUpBar.Value = 0;
			// 
			// mainMenu1
			// 
			this.mainMenu1.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					  this.menuItem1,
																					  this.menuItem3,
																					  this.menuItem6,
																					  this.menuItem10});
			// 
			// menuItem1
			// 
			this.menuItem1.Index = 0;
			this.menuItem1.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					  this.menuItem2});
			this.menuItem1.Text = "Session";
			// 
			// menuItem2
			// 
			this.menuItem2.Index = 0;
			this.menuItem2.Text = "Quit";
			this.menuItem2.Click += new System.EventHandler(this.menuItem2_Click);
			// 
			// menuItem3
			// 
			this.menuItem3.Index = 1;
			this.menuItem3.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					  this.menuItem4,
																					  this.menuItem5,
																					  this.menuItem7});
			this.menuItem3.Text = "Connection";
			// 
			// menuItem4
			// 
			this.menuItem4.Index = 0;
			this.menuItem4.Text = "Settings";
			this.menuItem4.Click += new System.EventHandler(this.menuItem4_Click);
			// 
			// menuItem5
			// 
			this.menuItem5.Enabled = false;
			this.menuItem5.Index = 1;
			this.menuItem5.Text = "Connect";
			this.menuItem5.Click += new System.EventHandler(this.menuItem5_Click);
			// 
			// menuItem7
			// 
			this.menuItem7.Index = 2;
			this.menuItem7.Text = "Terminal";
			this.menuItem7.Click += new System.EventHandler(this.menuItem7_Click);
			// 
			// menuItem6
			// 
			this.menuItem6.Index = 2;
			this.menuItem6.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					  this.menuItem9});
			this.menuItem6.Text = "Device";
			// 
			// menuItem9
			// 
			this.menuItem9.Index = 0;
			this.menuItem9.Text = "Calibrate";
			this.menuItem9.Click += new System.EventHandler(this.menuItem9_Click);
			// 
			// menuItem10
			// 
			this.menuItem10.Index = 3;
			this.menuItem10.Text = "About";
			this.menuItem10.Click += new System.EventHandler(this.menuItem10_Click);
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.BackColor = System.Drawing.Color.White;
			this.ClientSize = new System.Drawing.Size(314, 303);
			this.Controls.Add(this.leftUpBar);
			this.Controls.Add(this.leftDownBar);
			this.Controls.Add(this.rightDownBar);
			this.Controls.Add(this.rightUpBar);
			this.Cursor = System.Windows.Forms.Cursors.Arrow;
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
			this.MaximumSize = new System.Drawing.Size(600, 480);
			this.Menu = this.mainMenu1;
			this.Name = "Form1";
			this.Text = "MInaRo";
			this.Load += new System.EventHandler(this.MainClass_Load);
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() 
		{
			Application.Run(new Form1());			
		}

		private void MainClass_Load(object sender, System.EventArgs e)
		{
			if(!myJStick.InitDirectInput())
			{
				MessageBox.Show("No input device was found!");
			}
			else
			{
				this.socket = new UDPClientASocket();
				//Start controller timer
				timer1.Start();
			}
		}

		private void timer1_Tick(object sender, System.EventArgs e)
		{
			//Get joystick information
			try
			{
				posXY = myJStick.GetXY();
				POV = myJStick.GetPOV();
				posXY[0] *= this.axisFRSign[0]; 
				posXY[1] *= this.axisFRSign[1]; 
				posXY[2] *= this.axisFRSign[2]; 
				posXY[3] *= this.axisFRSign[3]; 
				buttonString = myJStick.GetButtons();
			}
			catch(Exception GetE)
			{
				//MessageBox.Show(GetE.ToString());
			}
			
			switch(axisCount)
			{
				case 1:
					JoystickControl();
					break;
				case 2:
					GamePadControl();
					break;
			}
			
			//Calculate & Refresh screen data
			#region Display Calculations
			x = (this.ClientSize.Width * (0.5 + posXY[0]*0.00125));
			y = (this.ClientSize.Height * (0.5 + posXY[1]*0.00125));
			
			posString = "X = " + x + "  Y = " + y;
			//Circle Limit processing
			if(!guiCircle.IsPointWithin(x, y))
			{
				float[] secXY = guiCircle.RadialIntersection((float)x, (float)y);
				x= secXY[0];
				y= secXY[1];
			}

			#endregion
			this.Refresh();
		}
		
		private void JoystickControl()
		{
			#region Joystick Control
			//Position calculation
			int x1 = (int)(posXY[0] * Math.Cos(rad45) + posXY[1] * (Math.Sin( rad45 )));
			int y1 = (int)(posXY[0] * Math.Sin(rad45) * (-1) + posXY[1] * (Math.Cos( rad45 )));
			
			float[] fposXY = new float[2];
			if(!limitCircle.IsPointWithin(x1, y1))
			{
				fposXY = limitCircle.RadialIntersection(x1, y1);
				posXY[0] = (int)fposXY[0];
				posXY[1] = (int)fposXY[1];
			}
			else
			{
				posXY[0] = x1;
				posXY[1] = y1;
			}

			if( (posXY[0]*posXY[1]) < 0 )
			{
				posXY[0] *= -1;
				posXY[1] *= -1;
			}
			if( posXY[0]>0 && posXY[1]>0)
			{
				int temp = posXY[0];
				posXY[0] = posXY[1];
				posXY[1] = temp;
			}
				
			if( Math.Abs(posXY[0]) > 512 )
			{
				if(posXY[0]<0)
					posXY[0] = 512 * (-1);
				else
					posXY[0] = 512;
			}
			if( Math.Abs(posXY[1]) > 512 )
			{
				if(posXY[1]<0)
					posXY[1] = 512 * (-1);
				else
					posXY[1] = 512;
			}

			//Movement Interface
			if(posXY[1] >= 0)
			{
				this.leftUpBar.Value = posXY[1];
				this.leftDownBar.Value = 0;
			}
			else
			{
				this.leftDownBar.Value = posXY[1]*(-1);
				this.leftUpBar.Value = 0;
			}
			
			if(posXY[0] >= 0)
			{
				this.rightUpBar.Value = posXY[0];
				this.rightDownBar.Value = 0;
			}
			else
			{
				this.rightDownBar.Value = posXY[0]*(-1);
				this.rightUpBar.Value = 0;
			}
			
			//Movement Message Build
	/*		string strCommand= "raw 5da0";
			if(posXY[1] >= 0)
				strCommand += "01";
			else
				strCommand += "02";

			strCommand += Math.Abs(posXY[1]).ToString("x4");
			if(strCommand != this.lastLeftCommand)
			{
				if(this.socket.Connected)
				{
					try{ this.socket.SendMsg(strCommand); }
					catch(System.Exception){ //MessageBox.Show("Network connection error");
					}
					this.lastLeftCommand = strCommand;
				}
			}
	
			strCommand= "raw 5d0a";
			if(posXY[0] >= 0)
				strCommand += "01";
			else
				strCommand += "02";

			strCommand += Math.Abs(posXY[0]).ToString("x4");
			if(strCommand != this.lastRightCommand)
			{
				if(this.socket.Connected)
				{
					try{ this.socket.SendMsg(strCommand); }
					catch(System.Exception){ //MessageBox.Show("Network connection error");

					}
					this.lastRightCommand = strCommand;
				}
			}*/

			string strCommand= "";

			if( (Math.Abs(posXY[0])>70) & (Math.Abs(posXY[1])>70) )
			{
				if( (Math.Abs((Math.Abs(posXY[0]))-(Math.Abs(posXY[1])))) >= 40 )
				{
					if( posXY[0]<posXY[1] ) 
					{
						strCommand = "turnr";
					}

					else
					{
						strCommand = "turnl";
					}
				}

				else
				{
					if( (posXY[0]<0) & (posXY[1]<0) )
					{
						strCommand = "stepb";
					}
					
					if( (posXY[0]>0) & (posXY[1]>0) )
					{
						if( (posXY[0]>400) & (posXY[1]>400) )
						{
							strCommand = "run";
						}

						if( (posXY[0]<280) & (posXY[1]<280) )
						{
							strCommand = "stepf";
						}
					}
					
					if( (posXY[0]<0) & (posXY[1]>0) )
					{
						strCommand = "stepr";
					}

					if( (posXY[0]>0) & (posXY[1]<0) )
					{
						strCommand = "stepl";
					}
				}
			}

			strCommand = strCommand + "\n";
			
	    	if(this.socket.Connected)
				{
	     			try{ this.socket.SendMsg(strCommand); }
			
			catch(System.Exception)
					{ //MessageBox.Show("Network connection error");
					}
					//Thread.Sleep(15);
				}

			//Shooter interface
			#region Shooter
			if(socket.Connected)
			{
				if( (buttonString.IndexOf("0")!=-1) )
				{
					if(lastShootCommand != "kickr")
					{
						socket.SendMsg("kickr\n");
						//lastShootCommand = "kickr";
					}
				}
				else
				{
					if(lastShootCommand != "")
					{
						//socket.SendMsg("");
						//lastShootCommand = "";
					}
				}
				if( buttonString.IndexOf(this.trainButton) != -1 )
				{
					if(lastTrainCommand != "kickl")
					{
						socket.SendMsg("kickl\n");
						//lastTrainCommand = "kickl";
					}
				}
				else
				{
					if(lastTrainCommand != "")
					{
						//socket.SendMsg("");
						//lastTrainCommand = "";
					}
				}
			}
			#endregion
			#endregion
		}

		private void GamePadControl()
		{
			double Y = 0;
			int sign = 1;
			if(posXY[1]<0) sign = -1;
			posXY[1] = (Math.Abs(posXY[1])>MAX_SPEED)? MAX_SPEED*sign: posXY[1];
			sign = 1;
			if(posXY[3]<0) sign = -1;
			posXY[3] = (Math.Abs(posXY[3])>MAX_SPEED)? MAX_SPEED*sign: posXY[3];
			#region Gamepad Control
			if( POV == -1 )
			{
				#region Analog Control
				//Left Motor command build
				string strCommand = "";
				//Movement Interface
				if(posXY[1] >= 0)
				{
					this.leftUpBar.Value = posXY[1];
					this.leftDownBar.Value = 0;
				}
				else
				{
					this.leftDownBar.Value = posXY[1]*(-1);
					this.leftUpBar.Value = 0;
				}
			
				if(posXY[3] >= 0)
				{
					this.rightUpBar.Value = posXY[3];
					this.rightDownBar.Value = 0;
				}
				else
				{
					this.rightDownBar.Value = posXY[3]*(-1);
					this.rightUpBar.Value = 0;
				}

				//Message Build
				
				if( (Math.Abs(posXY[2])>110) & (Math.Abs(posXY[3])<110))
				{
					if( posXY[2]>0 ) strCommand += "stepr";
					else		  strCommand += "stepl";
				}

				if( (Math.Abs(posXY[2])<110) & (Math.Abs(posXY[3])>110))
				{
					if( posXY[3]>0 ) strCommand += "stepf";
					else			 strCommand += "stepb";
				}

				if( (Math.Abs(posXY[2])>110) & (Math.Abs(posXY[3])>110))
					if( (Math.Abs((Math.Abs(posXY[2]))-(Math.Abs(posXY[3])))) <= 40 )
					{			
						if( (posXY[2]>0) & (posXY[3]>0) ) strCommand += "turnr";
						if( (posXY[2]<0) & (posXY[3]>0) ) strCommand += "turnl";
					}

				strCommand += "\n";

				Y = (posXY[1]>this.gamepadThold)? 170*Math.Log(0.2*(Math.Abs(posXY[1]))): 0;
				//Y = (posXY[1]>this.leftAnalogMin)? 0.5*Math.Exp( (Math.Abs(posXY[1])+250)/50): 0;
				//strCommand += ((int)Y).ToString("x4");
                
				if(strCommand != lastLeftCommand)
				{
					if(socket.Connected)
					{
						socket.SendMsg(strCommand);
						lastLeftCommand = strCommand;
					}
				}

				//Right Motor command build
				
				if( Math.Abs(posXY[0])>110 )
				{
					if( posXY[0]>0 ) strCommand += "keepr";
					if( posXY[0]<0 ) strCommand += "keepl";
				}

				if( posXY[1]>110 ) strCommand += "run";

				strCommand += "\n                                                                                                   ";

				Y = (posXY[3]>this.gamepadThold)? 170*Math.Log(0.2*(Math.Abs(posXY[3]))): 0;
				//Y = (posXY[3]>this.leftAnalogMin)? 0.5*Math.Exp( (Math.Abs(posXY[3])+250)/50): 0;
				//strCommand += ((int)Y).ToString("x4");
				
				if(strCommand != lastRightCommand)
				{
					if(socket.Connected)
					{
						socket.SendMsg(strCommand);
						lastRightCommand = strCommand;
					}
				}
				
				#endregion
			}
			else
			{
				try
				{
					switch(POV)
					{
						case 0:
							socket.SendMsg("1");
							//lastLeftCommand = "raw 5da001017f";
							//socket.SendMsg("raw 5d0a01017f");
							//lastRightCommand = "raw 5d0a01017f";
							break;
						case 4500:
							socket.SendMsg("2");
							//lastLeftCommand = "raw 5da00101ff";
							//socket.SendMsg("raw 5d0a010100");
							//lastRightCommand = "raw 5d0a010100";
							break;
						case 9000:
							socket.SendMsg("3");
							//lastLeftCommand = "raw 5da0010100";
							//socket.SendMsg("raw 5d0a020100");
							//lastRightCommand = "raw 5d0a020100";
							break;
						case 13500:
							socket.SendMsg("4");
							//lastLeftCommand = "raw 5da00101ff";
							//socket.SendMsg("raw 5d0a020100");
							//lastRightCommand = "raw 5d0a010100";
							break;
						case 18000:
							socket.SendMsg("5");
							//lastLeftCommand = "raw 5da002017f";
							//socket.SendMsg("raw 5d0a02017f");
							//lastRightCommand = "raw 5d0a02017f";
							break;
						case 22500:
							socket.SendMsg("6");
							//lastLeftCommand = "raw 5da0020122";
							//socket.SendMsg("raw 5d0a0201ff");
							//lastRightCommand = "raw 5d0a0201ff";
							break;
						case 27000:
							socket.SendMsg("7");
							//lastLeftCommand = "raw 5da0020100";
							//socket.SendMsg("raw 5d0a010100");
							//lastRightCommand = "raw 5d0a010100";
							break;
						case 31500:
							socket.SendMsg("8");
							//lastLeftCommand = "raw 5da0010122";
							//socket.SendMsg("raw 5d0a0101ff");
							//lastRightCommand = "raw 5d0a0101ff";
							break;
						default:
							break;
					}
				}
				catch(System.Exception)
				{
					MessageBox.Show("Network connection error.");
				}
			}

			//Shooter command check
			try
			{
				#region Shooter
				if(socket.Connected)
				{
					if( (buttonString.IndexOf("6")!=-1) || ( buttonString.IndexOf("7") != -1) )
					{
						if(lastShootCommand != "kickr")
						{
							socket.SendMsg("kickr\n");
							//lastShootCommand = "shooton";
						}
					}
					/*else
					{
						if(lastShootCommand != "")
						{
							socket.SendMsg("");
							lastShootCommand = "shootoff";
						}
					}*/
					if( buttonString.IndexOf(this.trainButton) != -1 )
					{
						if(lastTrainCommand != "kickl")
						{
							socket.SendMsg("kickl\n");
							//lastTrainCommand = "tron";
						}
					}
					/*else
					{
						if(lastTrainCommand != "troff")
						{
							socket.SendMsg("troff");
							lastTrainCommand = "troff";
						}
					}*/
				}
				#endregion							
				#endregion
			}
			catch(System.Exception)
			{
				//MessageBox.Show("Network Connection Error");
			}
		}
		private void menuItem2_Click(object sender, System.EventArgs e)
		{
			this.Close();
		}

		private void menuItem4_Click(object sender, System.EventArgs e)
		{
			ConnectionSetupForm connSet;
			if(this.remoteIPString == null)
				connSet = new ConnectionSetupForm();
			else
				connSet = new ConnectionSetupForm(this.remoteIPString, this.sendPort, this.rcvPort);
			if(connSet.ShowDialog() == DialogResult.OK)
			{
				if(this.socket.Connected)
					this.socket.Close();
				this.socket = null;
				this.socket = new UDPClientASocket();
				this.remoteIPString = connSet.ipTextBox.Text;
				this.sendPort = int.Parse(connSet.sendTextBox.Text);
				this.rcvPort = int.Parse(connSet.rcvTextBox.Text);
				try
				{
					this.socket.ConnectTo(this.remoteIPString, this.sendPort, this.rcvPort);
					this.menuItem5.Text = "Disconnect";
					this.menuItem5.Enabled = true;
				}
				catch(System.OverflowException)
				{
					MessageBox.Show("Not a valid port Number\r\nPort Range: 0-65536");
				}
				catch(System.FormatException)
				{
					MessageBox.Show("Not a valid IP Address\r\nOR ports must be integer numbers");
				}
				catch(System.Net.Sockets.SocketException)
				{
					MessageBox.Show("Remote Host could not be reached!");
				}
			}
		}

		private void menuItem10_Click(object sender, System.EventArgs e)
		{
			AboutForm about = new AboutForm();
			about.ShowDialog();
		}

		private void menuItem5_Click(object sender, System.EventArgs e)
		{
			switch(this.menuItem5.Text)
			{
				case "Connect":
					this.socket = null;
					this.socket = new UDPClientASocket();
					this.socket.ConnectTo(this.remoteIPString,	this.sendPort, this.rcvPort);
					this.menuItem5.Text = "Disconnect";
					break;
				case "Disconnect":
					this.socket.Close();
					this.menuItem5.Text = "Connect";
					break;
			}
		}

		private void menuItem9_Click(object sender, System.EventArgs e)
		{
			ChooseDeviceForm chooseDev = new ChooseDeviceForm();
			if(chooseDev.ShowDialog() == DialogResult.OK && chooseDev.selectedDevice != null)
			{
				this.myJStick.InitDirectInput(chooseDev.selectedDevice);
				CalibrationForm calibForm = new CalibrationForm(chooseDev.selectedDevice);
				if(calibForm.ShowDialog() == DialogResult.OK)
				{
					axisCount = calibForm.axisCount;
					myJStick.SetX1Axis(calibForm.axisIndex[0]);										myJStick.SetY1Axis(calibForm.axisIndex[1]);
					myJStick.SetX2Axis(calibForm.axisIndex[2]);
					myJStick.SetY2Axis(calibForm.axisIndex[3]);
					this.axisFRSign[0] = calibForm.axisFRSign[0];
					this.axisFRSign[1] = calibForm.axisFRSign[1];
					this.axisFRSign[2] = calibForm.axisFRSign[2];
					this.axisFRSign[3] = calibForm.axisFRSign[3];
					this.trainButton = calibForm.trainButton;
				}
				calibForm = null;
			}
			chooseDev = null;
		}

		private void menuItem7_Click(object sender, System.EventArgs e)
		{
			Terminal term = new Terminal(this.socket);
			term.Show();
		}

	}
}