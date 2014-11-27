using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using Microsoft.DirectX.DirectInput;

namespace Minaro
{
	/// <summary>
	/// Summary description for ChooseDeviceForm.
	/// </summary>
	public class ChooseDeviceForm : System.Windows.Forms.Form
	{
		private System.Windows.Forms.ListBox deviceListBox;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Button done;
		public string selectedDevice;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public ChooseDeviceForm()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			SetDeviceList();	
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
			this.deviceListBox = new System.Windows.Forms.ListBox();
			this.label1 = new System.Windows.Forms.Label();
			this.done = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// deviceListBox
			// 
			this.deviceListBox.Location = new System.Drawing.Point(16, 32);
			this.deviceListBox.Name = "deviceListBox";
			this.deviceListBox.Size = new System.Drawing.Size(176, 95);
			this.deviceListBox.TabIndex = 0;
			this.deviceListBox.DoubleClick += new System.EventHandler(this.deviceListBox_DoubleClick);
			// 
			// label1
			// 
			this.label1.ForeColor = System.Drawing.Color.FromArgb(((System.Byte)(192)), ((System.Byte)(0)), ((System.Byte)(0)));
			this.label1.Location = new System.Drawing.Point(16, 8);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(192, 16);
			this.label1.TabIndex = 1;
			this.label1.Text = "The current available devices are:";
			// 
			// done
			// 
			this.done.BackColor = System.Drawing.Color.FromArgb(((System.Byte)(192)), ((System.Byte)(0)), ((System.Byte)(0)));
			this.done.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.done.ForeColor = System.Drawing.Color.FromArgb(((System.Byte)(224)), ((System.Byte)(224)), ((System.Byte)(224)));
			this.done.Location = new System.Drawing.Point(72, 136);
			this.done.Name = "done";
			this.done.Size = new System.Drawing.Size(64, 24);
			this.done.TabIndex = 2;
			this.done.Text = "Done";
			this.done.Click += new System.EventHandler(this.done_Click);
			// 
			// ChooseDeviceForm
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.BackColor = System.Drawing.Color.White;
			this.ClientSize = new System.Drawing.Size(208, 171);
			this.Controls.Add(this.done);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.deviceListBox);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "ChooseDeviceForm";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Choose Device";
			this.ResumeLayout(false);

		}
		#endregion

		public void SetDeviceList()
		{
			foreach (DeviceInstance instance in Manager.GetDevices(DeviceClass.GameControl, EnumDevicesFlags.AttachedOnly))
			{
				this.deviceListBox.Items.Add(instance.InstanceName);
			}
			if(deviceListBox.Items.Count > 0)
				deviceListBox.SelectedIndex = 0;
		}

		private void done_Click(object sender, System.EventArgs e)
		{
			this.selectedDevice = (string)deviceListBox.SelectedItem;
		}

		private void deviceListBox_DoubleClick(object sender, EventArgs e)
		{
			this.selectedDevice = (string)deviceListBox.SelectedItem;
		}
	}
}
