using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using ZalSystem;

namespace Minaro
{
	/// <summary>
	/// Summary description for Terminal.
	/// </summary>
	public class Terminal : System.Windows.Forms.Form
	{
		private System.Windows.Forms.TextBox sendTextBox;
		private System.Windows.Forms.Button button1;
		private System.Windows.Forms.TextBox rcvTextBox;

		public UDPClientASocket udpSock;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public Terminal(UDPClientASocket masterSocket)
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();
			udpSock = masterSocket;
			udpSock.DataReceived += new ZalSystem.UDPClientASocket.DataReceivedEventHandler(udpSock_DataReceived);
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
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
			this.sendTextBox = new System.Windows.Forms.TextBox();
			this.button1 = new System.Windows.Forms.Button();
			this.rcvTextBox = new System.Windows.Forms.TextBox();
			this.SuspendLayout();
			// 
			// sendTextBox
			// 
			this.sendTextBox.Location = new System.Drawing.Point(16, 16);
			this.sendTextBox.Name = "sendTextBox";
			this.sendTextBox.Size = new System.Drawing.Size(96, 20);
			this.sendTextBox.TabIndex = 0;
			this.sendTextBox.Text = "mv 1 1";
			// 
			// button1
			// 
			this.button1.BackColor = System.Drawing.Color.FromArgb(((System.Byte)(192)), ((System.Byte)(0)), ((System.Byte)(0)));
			this.button1.ForeColor = System.Drawing.Color.White;
			this.button1.Location = new System.Drawing.Point(128, 16);
			this.button1.Name = "button1";
			this.button1.Size = new System.Drawing.Size(48, 24);
			this.button1.TabIndex = 1;
			this.button1.Text = "Send";
			this.button1.Click += new System.EventHandler(this.button1_Click);
			// 
			// rcvTextBox
			// 
			this.rcvTextBox.Location = new System.Drawing.Point(16, 56);
			this.rcvTextBox.Multiline = true;
			this.rcvTextBox.Name = "rcvTextBox";
			this.rcvTextBox.Size = new System.Drawing.Size(160, 56);
			this.rcvTextBox.TabIndex = 2;
			this.rcvTextBox.Text = "RecievedMessage";
			// 
			// Terminal
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.BackColor = System.Drawing.Color.White;
			this.ClientSize = new System.Drawing.Size(192, 125);
			this.Controls.Add(this.rcvTextBox);
			this.Controls.Add(this.button1);
			this.Controls.Add(this.sendTextBox);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
			this.Name = "Terminal";
			this.Opacity = 0.8;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Terminal";
			this.ResumeLayout(false);

		}
		#endregion

		private void button1_Click(object sender, System.EventArgs e)
		{
			if(this.sendTextBox.Text != "" && udpSock!=null && udpSock.Connected)
			{
				udpSock.SendMsg(sendTextBox.Text);
			}
		}

		private void udpSock_DataReceived(object sender, SocketPack socData)
		{
			string rcvData = this.udpSock.DecodeBytesToString(socData.dataBuffer);
			this.rcvTextBox.Text += rcvData + "\r\n";
		}
	}
}
