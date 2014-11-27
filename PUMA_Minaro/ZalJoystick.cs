using System;
using Microsoft.DirectX.DirectInput;
using Microsoft.DirectX;
using System.Windows.Forms;


namespace ZalSystem
{
	/// <summary>
	/// Summary description for Class1.
	/// </summary>
	public class ZalJoystick
	{
		public static JoystickState state = new JoystickState();
		public Device applicationDevice = null;
		private Control appHandler;
		private int _range;
		private int[] Axis;
		private int indexX1;
		private int indexY1;
		private int indexX2;
		private int indexY2;
		private int limitThreshold;

		public int LimitThreshold
		{
			get{ return this.limitThreshold; }
			set{ limitThreshold = value; }
		}
		public ZalJoystick(Control handle)
		{
			_range = 1000;
			indexX1 = 21;
			indexY1 = 22;
			indexX2 = 12;
			indexY2 = 13;
			Axis = new int[26];
			appHandler = handle;
		}

		public ZalJoystick(Control handle, int range)
		{
			_range = range;
			indexX1 = 21;
			indexY1 = 22;
			indexX2 = 12;
			indexY2 = 13;
			Axis = new int[28];
			limitThreshold = 100;
			appHandler = handle;
		}

		public void SetX1Axis(int index){ indexX1 = index; }
		
		public void SetY1Axis(int index){ indexY1 = index; }
		
		public void SetX2Axis(int index){ indexX2 = index; }

		public void SetY2Axis(int index){ indexY2 = index; }

		public int WhichAxIsMax()
		{	
			int i  = 0; 
			foreach(int a in Axis)
			{
				if(a == _range - limitThreshold)
					break;
				i++;
			}
			if(i == Axis.Length)
				i = -1;
			return i;
		}

		public int WhichAxIsMin()
		{	
			int i = 0; 
			foreach(int a in Axis)
			{
				if(a == -_range + limitThreshold)
					break;
				i++;
			}
			if(i == Axis.Length)
				i = -1;
			return i;
		}

		public bool InitDirectInput()
		{
			// Enumerate Joysticks in the system.
			foreach (DeviceInstance instance in Manager.GetDevices(DeviceClass.GameControl, EnumDevicesFlags.AttachedOnly))
			{
				// Create the device.  Just pick the last one
				applicationDevice = new Device(instance.InstanceGuid);
			}

			if ( applicationDevice == null )
			{
				return false;
			}

			// Set the data format to the c_dfDIJoystick pre-defined format.
			applicationDevice.SetDataFormat(DeviceDataFormat.Joystick);
			// Set the cooperative level for the device.
			//applicationDevice.SetCooperativeLevel(appHandler, CooperativeLevelFlags.Exclusive | CooperativeLevelFlags.Foreground);
			// Enumerate all the objects on the device.
			foreach (DeviceObjectInstance d in applicationDevice.Objects)
			{
				// For axes that are returned, set the DIPROP_RANGE property for the
				// enumerated axis in order to scale min/max values.

				if ( (d.ObjectId & (int)DeviceObjectTypeFlags.Axis) != 0 )
				{
					// Set the range for the axis.
					applicationDevice.Properties.SetRange(ParameterHow.ById, d.ObjectId, new InputRange(-_range, _range));
				}
				// Update the controls to reflect what
				// objects the device supports.
			}
			return true;
		}

		public bool InitDirectInput(string deviceName)
		{
			foreach (DeviceInstance instance in Manager.GetDevices(DeviceClass.GameControl, EnumDevicesFlags.AttachedOnly))
			{
				// Create the device.  Just pick the last one
				applicationDevice = new Device(instance.InstanceGuid);
				if(instance.InstanceName == deviceName)
					break;
			}

			if ( applicationDevice == null )
			{
				return false;
			}

			// Set the data format to the c_dfDIJoystick pre-defined format.
			applicationDevice.SetDataFormat(DeviceDataFormat.Joystick);
			// Set the cooperative level for the device.
			//applicationDevice.SetCooperativeLevel(appHandler, CooperativeLevelFlags.Exclusive | CooperativeLevelFlags.Foreground);
			// Enumerate all the objects on the device.
			foreach (DeviceObjectInstance d in applicationDevice.Objects)
			{
				// For axis that are returned, set the DIPROP_RANGE property for the enumerated axis in order to scale min/max values.

				if ( (d.ObjectId & (int)DeviceObjectTypeFlags.Axis) != 0 )
				{
					// Set the range for the axis.
					applicationDevice.Properties.SetRange(ParameterHow.ById, d.ObjectId, new InputRange(-_range, _range));
				}
				// Update the controls to reflect what objects the device supports.
			}
			return true;
		}

		public string[] GetDeviceList()
		{
			int i = 0;
			string[] deviceList = new string[Manager.Devices.Count-2];
			foreach (DeviceInstance instance in Manager.GetDevices(DeviceClass.GameControl, EnumDevicesFlags.AttachedOnly))
			{
				deviceList[i++] = instance.InstanceName;
			}
			return deviceList;
		}
		
		
		public int[] GetXY()
		{
			GetData();
			return new int[4]{Axis[indexX1], Axis[indexY1], Axis[indexX2], Axis[indexY2]};
		}

		public int GetPOV()
		{
			GetData();
			return (state.GetPointOfView())[0];
		}

		public string GetButtons()
		{
			string strText = "";
			byte[] buttons;

			GetData();

			buttons = state.GetButtons();

			int button = 0;
			foreach (byte b in buttons)
			{
				if ( (b & 0x80) != 0 )
					strText += button.ToString();
				button++;
			}
			return strText;
		}

		protected void GetData()
		{
			// Make sure there is a valid device.
			if ( applicationDevice == null )
				return;

			try
			{
				// Poll the device for info.
				applicationDevice.Poll();
			}
			catch(InputException inputex)
			{
				if ((inputex is NotAcquiredException) || (inputex is InputLostException))
				{
					// Check to see if either the app
					// needs to acquire the device, or
					// if the app lost the device to another
					// process.
					try
					{
						// Acquire the device.
						applicationDevice.Acquire();
					}
					catch(InputException)
					{
						// Failed to acquire the device.
						// This could be because the app
						// doesn't have focus.
						return;
					}
				}
                
			} //catch(InputException inputex)

			// Get the state of the device.
			try
			{
				state = applicationDevice.CurrentJoystickState;
				CollectAxis();
			}
			// Catch any exceptions. None will be handled here, 
			// any device re-aquisition will be handled above.  
			catch(InputException)
			{
				return;
			}
		}

		protected void CollectAxis()
		{
			Axis[0] = state.ARx;
			Axis[1] = state.ARy;
			Axis[2] = state.ARz;
			Axis[3] = state.AX;
			Axis[4] = state.AY;
			Axis[5] = state.AZ;
			Axis[6] = state.FRx;
			Axis[7] = state.FRy;
			Axis[8] = state.FRz;
			Axis[9] = state.FX;
			Axis[10] = state.FY;
			Axis[11] = state.FZ;
			Axis[12] = state.Rx;
			Axis[13] = state.Ry;
			Axis[14] = state.Rz;
			Axis[15] = state.VRx;
			Axis[16] = state.VRy;
			Axis[17] = state.VRz;
			Axis[18] = state.VX;
			Axis[19] = state.VY;
			Axis[20] = state.VZ;
			Axis[21] = state.X;
			Axis[22] = state.Y;
			Axis[23] = state.Z;
			int[] aux = state.GetSlider(); 
			Axis[24] = aux[0];
			Axis[25] = aux[1];
		}

		public void Unacquire()
		{
			if(this.applicationDevice != null)
				applicationDevice.Unacquire();
		}
	}
}
