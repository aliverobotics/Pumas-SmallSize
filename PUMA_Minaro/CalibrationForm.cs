using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using ZalSystem;

namespace Minaro
{
	/// <summary>
	/// Summary description for CalibrationForm.
	/// </summary>
	public class CalibrationForm : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Label label1;
		private string textAxis = "Which configuration would you prefer?\r\nFor a 1-Axis or 2-Axis device.";
		private string textFront = "Pull the joystick to the FRONT and keep pressed any button.";
		private string textRight = "Pull the joystick to the RIGHT and keep pressed any button.";
		private string textLFront = "Pull the LEFT joystick to the FRONT and keep pressed any button.";
		private string textLRight = "Pull the LEFT joystick to the RIGHT and keep pressed any button.";
		private string textRFront = "Pull the RIGHT joystick to the FRONT and keep pressed any button.";
		private string textRRight = "Pull the RIGHT joystick to the RIGHT and keep pressed any button.";
		private string textTButton = "Push the button that will be used to turn ON/OFF the TRAINING State";

		private enum Step
		{
			FRONT,
			RIGHT,
			LEFT_FRONT,
			LEFT_RIGHT,
			RIGHT_FRONT,
			RIGHT_RIGHT,
			TRAIN_BUTTON,
			DONE
		}
		public int axisCount;
		private Step currentStep;
		private ZalJoystick jStick;
		public int[] axisIndexSign;
		public int[] axisIndex;
		public int[] axisFRSign;
		public string trainButton;
		private System.Windows.Forms.Button contCancelButton;
		private System.Windows.Forms.RadioButton radioButton1;
		private System.Windows.Forms.RadioButton radioButton2;
		private System.Windows.Forms.Timer buttonTimer;
		private System.ComponentModel.IContainer components;

		public CalibrationForm(string chosenDevice)
		{
			InitializeComponent();
			this.label1.Text = this.textAxis;
			this.axisCount = -1;
			this.axisIndexSign = new int[4];
			this.axisIndex = new int[4];
			this.axisFRSign = new int[4];
			this.trainButton = "";
			this.jStick = new ZalJoystick(this);
			this.jStick.InitDirectInput(chosenDevice);
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
			this.components = new System.ComponentModel.Container();
			this.label1 = new System.Windows.Forms.Label();
			this.contCancelButton = new System.Windows.Forms.Button();
			this.radioButton1 = new System.Windows.Forms.RadioButton();
			this.radioButton2 = new System.Windows.Forms.RadioButton();
			this.buttonTimer = new System.Windows.Forms.Timer(this.components);
			this.SuspendLayout();
			// 
			// label1
			// 
			this.label1.Location = new System.Drawing.Point(16, 8);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(240, 32);
			this.label1.TabIndex = 0;
			// 
			// contCancelButton
			// 
			this.contCancelButton.BackColor = System.Drawing.Color.FromArgb(((System.Byte)(192)), ((System.Byte)(0)), ((System.Byte)(0)));
			this.contCancelButton.ForeColor = System.Drawing.Color.White;
			this.contCancelButton.Location = new System.Drawing.Point(96, 136);
			this.contCancelButton.Name = "contCancelButton";
			this.contCancelButton.Size = new System.Drawing.Size(72, 24);
			this.contCancelButton.TabIndex = 2;
			this.contCancelButton.Text = "Next";
			this.contCancelButton.Click += new System.EventHandler(this.contCancelButton_Click);
			// 
			// radioButton1
			// 
			this.radioButton1.Checked = true;
			this.radioButton1.Location = new System.Drawing.Point(88, 56);
			this.radioButton1.Name = "radioButton1";
			this.radioButton1.TabIndex = 3;
			this.radioButton1.TabStop = true;
			this.radioButton1.Text = "1 - Axis";
			// 
			// radioButton2
			// 
			this.radioButton2.Location = new System.Drawing.Point(88, 88);
			this.radioButton2.Name = "radioButton2";
			this.radioButton2.TabIndex = 3;
			this.radioButton2.Text = "2 - Axis";
			// 
			// buttonTimer
			// 
			this.buttonTimer.Interval = 1000;
			this.buttonTimer.Tick += new System.EventHandler(this.buttonTimer_Tick);
			// 
			// CalibrationForm
			// 
			this.AutoScale = false;
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.BackColor = System.Drawing.Color.White;
			this.ClientSize = new System.Drawing.Size(272, 165);
			this.Controls.Add(this.radioButton1);
			this.Controls.Add(this.radioButton2);
			this.Controls.Add(this.contCancelButton);
			this.Controls.Add(this.label1);
			this.ForeColor = System.Drawing.Color.FromArgb(((System.Byte)(192)), ((System.Byte)(0)), ((System.Byte)(0)));
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
			this.Name = "CalibrationForm";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Calibration";
			this.ResumeLayout(false);

		}
		#endregion

		private void contCancelButton_Click(object sender, System.EventArgs e)
		{
			switch(contCancelButton.Text)
			{
				case "Next":
					contCancelButton.Text = "Cancel";
					if(this.radioButton1.Checked)
					{
						this.axisCount = 1;
						currentStep = Step.FRONT;
						this.label1.Text = this.textFront;
					}
					if(this.radioButton2.Checked)
					{
						this.axisCount = 2;
						currentStep = Step.LEFT_FRONT;
						this.label1.Text = this.textLFront;
					}

					radioButton1.Visible = false;
					radioButton2.Visible = false;
						
					this.buttonTimer.Start();
					break;
				case "Cancel":
					this.DialogResult = System.Windows.Forms.DialogResult.Cancel;
					break;
			}
		}

		private void buttonTimer_Tick(object sender, System.EventArgs e)
		{
			if( (this.trainButton = this.jStick.GetButtons()) != "")
			{
				switch(currentStep)
				{
					case Step.FRONT:
						if( (axisIndex[1] = this.jStick.WhichAxIsMin()) < 0)
						{
							axisIndex[1] = this.jStick.WhichAxIsMax();
						}else
						{
							axisFRSign[1] = -1;
						}
						currentStep = Step.RIGHT;
						this.label1.Text = this.textRight;
						break;
					case Step.RIGHT:
						if( (axisIndex[0] = this.jStick.WhichAxIsMin()) < 0)
						{
							axisIndex[0] = this.jStick.WhichAxIsMax();
							axisFRSign[0] = 1;
						}
						else
						{
							axisFRSign[0] = -1;
						}
						currentStep = Step.TRAIN_BUTTON;
						this.label1.Text = this.textTButton;
						break;
					case Step.LEFT_FRONT:
						if( (axisIndex[1] = this.jStick.WhichAxIsMin()) < 0)
						{
							axisIndex[1] = this.jStick.WhichAxIsMax();
							axisFRSign[1] = 1;
						}
						else
						{
							axisFRSign[1] = -1;
						}
						currentStep = Step.LEFT_RIGHT;
						this.label1.Text = this.textLRight;
						break;
					case Step.LEFT_RIGHT:
						if( (axisIndex[0] = this.jStick.WhichAxIsMin()) < 0)
						{
							axisIndex[0] = this.jStick.WhichAxIsMax();
							axisFRSign[0] = 1;
						}
						else
						{
							axisFRSign[0] = -1;
						}
						currentStep = Step.RIGHT_FRONT;
						this.label1.Text = this.textRFront;
						break;
					case Step.RIGHT_FRONT:
						if( (axisIndex[3] = this.jStick.WhichAxIsMin()) < 0)
						{
							axisIndex[3] = this.jStick.WhichAxIsMax();
							axisFRSign[3] = 1;
						}
						else
						{
							axisFRSign[3] = -1;
						}
						currentStep = Step.RIGHT_RIGHT;
						this.label1.Text = this.textRRight;
						break;
					case Step.RIGHT_RIGHT:
						if( (axisIndex[2] = this.jStick.WhichAxIsMin()) < 0)
						{
							axisIndex[2] = this.jStick.WhichAxIsMax();
							axisFRSign[2] = 1;
						}
						else
						{
							axisFRSign[2] = -1;
						}
						currentStep = Step.TRAIN_BUTTON;
						this.label1.Text = this.textTButton;					
						break;
					case Step.TRAIN_BUTTON:
						currentStep = Step.DONE;
						this.buttonTimer.Stop();
						this.DialogResult = System.Windows.Forms.DialogResult.OK;
						break;
				}
			}
		}

	}
}