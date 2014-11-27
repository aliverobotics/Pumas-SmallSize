using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

namespace Minaro
{
	/// <summary>
	/// Summary description for ConnectionSetupForm.
	/// </summary>
	public class ConnectionSetupForm : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label3;
		public System.Windows.Forms.Button button1;
		public System.Windows.Forms.TextBox ipTextBox;
		public System.Windows.Forms.TextBox sendTextBox;
		public System.Windows.Forms.TextBox rcvTextBox;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public ConnectionSetupForm()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();
		}

		public ConnectionSetupForm(string ipAddress, int send, int rcv)
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			this.ipTextBox.Text = ipAddress;
			this.sendTextBox.Text = send.ToString();
			this.rcvTextBox.Text = rcv.ToString();
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
			this.label1 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.ipTextBox = new System.Windows.Forms.TextBox();
			this.sendTextBox = new System.Windows.Forms.TextBox();
			this.rcvTextBox = new System.Windows.Forms.TextBox();
			this.button1 = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// label1
			// 
			this.label1.ForeColor = System.Drawing.Color.FromArgb(((System.Byte)(192)), ((System.Byte)(0)), ((System.Byte)(0)));
			this.label1.Location = new System.Drawing.Point(16, 8);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(64, 16);
			this.label1.TabIndex = 0;
			this.label1.Text = "Robot I.P.";
			// 
			// label2
			// 
			this.label2.ForeColor = System.Drawing.Color.FromArgb(((System.Byte)(192)), ((System.Byte)(0)), ((System.Byte)(0)));
			this.label2.Location = new System.Drawing.Point(16, 40);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(64, 16);
			this.label2.TabIndex = 0;
			this.label2.Text = "Send Port";
			// 
			// label3
			// 
			this.label3.ForeColor = System.Drawing.Color.FromArgb(((System.Byte)(192)), ((System.Byte)(0)), ((System.Byte)(0)));
			this.label3.Location = new System.Drawing.Point(16, 72);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(64, 16);
			this.label3.TabIndex = 0;
			this.label3.Text = "Recv Port";
			// 
			// ipTextBox
			// 
			this.ipTextBox.Location = new System.Drawing.Point(88, 8);
			this.ipTextBox.Name = "ipTextBox";
			this.ipTextBox.Size = new System.Drawing.Size(112, 20);
			this.ipTextBox.TabIndex = 1;
			this.ipTextBox.Text = "192.168.2.25";
			// 
			// sendTextBox
			// 
			this.sendTextBox.Location = new System.Drawing.Point(88, 40);
			this.sendTextBox.Name = "sendTextBox";
			this.sendTextBox.Size = new System.Drawing.Size(56, 20);
			this.sendTextBox.TabIndex = 1;
			this.sendTextBox.Text = "8001";
			// 
			// rcvTextBox
			// 
			this.rcvTextBox.Location = new System.Drawing.Point(88, 72);
			this.rcvTextBox.Name = "rcvTextBox";
			this.rcvTextBox.Size = new System.Drawing.Size(56, 20);
			this.rcvTextBox.TabIndex = 1;
			this.rcvTextBox.Text = "8002";
			// 
			// button1
			// 
			this.button1.BackColor = System.Drawing.Color.FromArgb(((System.Byte)(192)), ((System.Byte)(0)), ((System.Byte)(0)));
			this.button1.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.button1.ForeColor = System.Drawing.Color.FromArgb(((System.Byte)(224)), ((System.Byte)(224)), ((System.Byte)(224)));
			this.button1.Location = new System.Drawing.Point(72, 104);
			this.button1.Name = "button1";
			this.button1.Size = new System.Drawing.Size(72, 24);
			this.button1.TabIndex = 2;
			this.button1.Text = "Connect";
			// 
			// ConnectionSetupForm
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.BackColor = System.Drawing.Color.White;
			this.ClientSize = new System.Drawing.Size(206, 131);
			this.Controls.Add(this.button1);
			this.Controls.Add(this.ipTextBox);
			this.Controls.Add(this.sendTextBox);
			this.Controls.Add(this.rcvTextBox);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.label3);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
			this.Name = "ConnectionSetupForm";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Connection Setup";
			this.ResumeLayout(false);

		}
		#endregion
	}
}
