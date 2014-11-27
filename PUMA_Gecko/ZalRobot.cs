using System;
using System.IO;
using System.Threading;
using System.Text;

namespace ZalSystem
{
	/// <summary>
	/// Makes easier to program a controllng application
	/// for the "ROBOCUP" robots developed in:
	/// Lab. Biorobotica
	/// Facultad de Ingenieria
	/// U.N.A.M.
	/// </summary>
	public class ZalRobot
	{
		public int[] Direction;			//Direction Vector of robot
		public int[] Position;			//Actual Position of Robot
		public int X;					//X Position
		public int Y;					//Y Position
		public string name;				//Robot's string name
		public int idNum;				//Robots identification number
		public float robotRadio;		//Radio of the robot	
		public float tireRadio;			//Radio of robot's tire
		public float turningRatio;		// robotRadio/tireRadio
		public SerialPort sPort;		//Serial port of pocket pc
		public int[] turnPulsesChart;	//Array of pulses needed to turn in steps of 10°
		public int[] movePulsesChart;   //Array of pulses needed to move the robot in steps of 5 cm

		//Strings sent through serial port to control the robot
		private const string SHOOTON	= "FF00000001";
		private const string SHOOTOFF	= "FF00000000";
		private const string SONARREQ	= "5C01";
		private const string IRREQ		= "5CFE";
		private const string TOUCHREQ	= "5C04";
		private const string CAMERAREQ	= "5CFB";
		private const string LINEREQ	= "5C20";
		private const string COMPASSREQ = "5CDF";
		private const string LASERREQ	= "5C80";

		#region ComponentInitialization
		private void IntializeComponents()
		{
			//Serial port 
			this.sPort = new SerialPort(19200, 8, 0, 1);

			//Turning angle constant 
			robotRadio = 8.35F;
			tireRadio  = 1.85F;
			turningRatio = (tireRadio/robotRadio) * 3;
			
			//Turning pulses chart from 0°-180° [inc = 5°]
			turnPulsesChart = new int[]{0, 15, 30, 60, 80, 100, 125, 150, 175, 200, 225, 250, 280,
										310, 345, 380, 440, 500, 530, 560, 615, 670, 725, 780, 845, 900, 960, 
										1020, 1080, 1140, 1200, 1260, 1280, 1300, 1400, 1500, 1600};

			//Advance pulses chart from 0[cm] - 50[cm] [inc = 5cm]
			movePulsesChart = new int[]{0, 200, 400, 700, 850, 1100, 2000, 
										2500, 3000, 3500, 4000, 4500,
										5000, 5500, 6000, 6500, 7000,
										7500, 8000, 8500, 9000};										
		}
		#endregion

		public ZalRobot()
		{
			IntializeComponents();
			
			this.name = "Gouki";
		}

		//Robot actions wrapped up in accesible methods
		#region Movement Methods
		//------------------------------------------------------------------------------------
		//Function: ShootOn
		//Purpose:  Activate the shooting device
		//Accepts:	Nothing
		//Returns:	Nothing
		//Throws:	
		//------------------------------------------------------------------------------------
		public void ShootOn()
		{
			if(sPort.Opened)
			{
				sPort.WriteString(SHOOTON);
			}
		}

		//------------------------------------------------------------------------------------
		//Function: ShootOff
		//Purpose:  Activate the shooting device
		//Accepts:	Nothing
		//Returns:	Nothing
		//Throws:	
		//------------------------------------------------------------------------------------
		public void ShootOff()
		{
			if(sPort.Opened)
			{
				sPort.WriteString(SHOOTOFF);
			}
		}
		
		//
		//pending
		//
		
		//------------------------------------------------------------------------------------
		//Function: TurnAndMove
		//Purpose:  Turns the amount of degrees then advances or backwards
		//Accepts:	Degrees to turn (int) and the distance to advance (int)
		//Returns:	Nothing
		//Throws:	
		//------------------------------------------------------------------------------------
		
		int degAngle;
		public string TurnAndMove(float fDistance, float radAngle)
		{
			int distance;
		
			string distHexString = "";
			string angleHexString = "";
			/*All calculations for distance are done in mm but input is in dc
			 *therefore we convert
			 *And all calculations for angles are done in deg but radians are 
			 *are needed, therefore we also conver this =/ */

				//Distance
				fDistance = fDistance * 100;
				distance = (int)fDistance;
				distHexString = this.GetDistPulses(distance).ToString("x4");

				//Angle
				radAngle = (radAngle * 180)/((float)Math.PI);
				degAngle = (int) radAngle;
				try{
					angleHexString = this.GetAnglePulses(degAngle).ToString("x4");
				}
				catch(Exception ex)
				{
					FileMode fMode;
					if(System.IO.File.Exists(@"\Program Files\UDPClient\errorparsed.txt"))
						fMode = FileMode.Append;
					else
						fMode = FileMode.CreateNew;
					FileStream fStream = new FileStream(@"\Program Files\UDPClient\errorparsed.txt",
						fMode, FileAccess.Write);
					StreamWriter fileWriter = new StreamWriter(fStream);
				
					fileWriter.Write( degAngle  + "\r\n" + angleHexString + "\r\n" + ex.ToString() + "\r\n" + System.DateTime.Now.ToString() + "\r\n");
		
					fileWriter.Close();
					fStream.Close();
					angleHexString = "0000";
				}

				string command = "a3" + distHexString + angleHexString;
				sPort.WriteString(command);
			
				string answerString = this.DecodeByteToString(sPort.ReadBuffer(64), System.Text.Encoding.ASCII);
				if(command == answerString.Substring(0, 10))
				{
					answerString = this.DecodeByteToString(sPort.ReadBuffer(64), System.Text.Encoding.ASCII);
					answerString = answerString.Substring(0, 10);
				}
				else
					answerString = "ERROR";
				return(answerString);	
		}

		/// <summary>
		/// Receives the distance in millimeters and returns the pulses needed
		/// </summary>
		/// <param name="decDistance"></param>
		/// <returns></returns>
		private int GetDistPulses(int decDistance)
		{
			int distancePulses;
			bool goForward = true;

			if(decDistance < 0)
				goForward = false;
			if(decDistance > 1000)
				decDistance = 1000;

			distancePulses = Math.Abs(decDistance);

			if((distancePulses%50)<=25)
				distancePulses = distancePulses - (distancePulses%50);
			else
				distancePulses = distancePulses + 50 - (distancePulses%50);
			
			distancePulses = distancePulses / 50;
			
			distancePulses = movePulsesChart[distancePulses];
			
			
			if(goForward)						  //0x0000 = 0(DEC) ;  0x8000 =  32768(DEC)
				distancePulses= distancePulses| 0x8000;  //1 bit direccion->15 bits de datos
			else
				distancePulses = distancePulses | 0;

			return distancePulses;
		}
		
		/// <summary>
		/// Receives the angle in degrees and returns the pulses needed
		/// </summary>
		/// <param name="degAngle"></param>
		/// <returns></returns>
		private int	GetAnglePulses(int degAngle)
		{
			int anglePulses;
			bool turnRight = false;

			if(degAngle < 0)
			{
				turnRight = true;
				degAngle = Math.Abs(degAngle);
			}
			//Set data in the range [0,360]
			while(degAngle > 180)
				degAngle = degAngle - 360;
			while(degAngle < -180)
				degAngle = degAngle + 360;

			anglePulses = (int)degAngle;
			if((anglePulses%5)<=2)
				anglePulses = anglePulses - (anglePulses%5);
			else
				anglePulses = anglePulses + ( 5 - (anglePulses%5));
			
			anglePulses = anglePulses / 5;
			anglePulses = turnPulsesChart[anglePulses];

			if(turnRight)						  //0x7000 = 28672(DEC) ;  0x8000 =  32768(DEC)
				anglePulses = anglePulses | 0;  //1 bit direccion->15 bits de datos
			else
				anglePulses = anglePulses | 0x8000;
			
			return anglePulses;
		}

		public string DecodeByteToString(byte[] byteArray, System.Text.Encoding encoding)
		{
			char[] chars = new char[byteArray.Length +  1];
			System.Text.Decoder d = encoding.GetDecoder();
			int charLen = d.GetChars(byteArray, 0, byteArray.Length, chars, 0);
			return new System.String(chars);
			
		}
		#endregion

		#region Other Commands
		//------------------------------------------------------------------------------------
		//Function: SetSpeed
		//Purpose:  Set the speed of the robot's motors
		//Accepts:	An integer value from 0-511
		//Returns:	Nothing
		//Throws:	
		//------------------------------------------------------------------------------------
		public string SetSpeed(int intSpeedVal)
		{
			string strSpeedVal = "";

			if(sPort.Opened)
			{
				if( intSpeedVal<=511 && intSpeedVal >= 0)
				{
					strSpeedVal = intSpeedVal.ToString("x4");
					sPort.WriteString("A001"+ strSpeedVal.Substring(2,2) + "A002" + strSpeedVal.Substring(0,2));
					Thread.Sleep(50);
					sPort.WriteString("0A01"+ strSpeedVal.Substring(2,2) + "0A02" + strSpeedVal.Substring(0,2));
				}
				else
				{
					throw new Exception("Speed value is out of range");
				}
			}

			return strSpeedVal;
		}

		//------------------------------------------------------------------------------------
		//Function: SetDistance
		//Purpose:  Sets the pulses for the robot's motors' encoders
		//Accepts:	
		//Returns:	Nothing
		//Throws:	
		//------------------------------------------------------------------------------------
		public void SetDistance(int distance)
		{
			if(distance > 65534)
				distance = 65534;
			else if(distance <0)
				distance = 0;

			string strHexPulses = distance.ToString("x4");
			sPort.WriteString("A003" + strHexPulses.Substring(2,2) + "A004" + strHexPulses.Substring(0,2));
			Thread.Sleep(50);
			sPort.WriteString("0A03" + strHexPulses.Substring(2,2) + "0A04" + strHexPulses.Substring(0,2));
			Thread.Sleep(50);
		}

		//------------------------------------------------------------------------------------
		//Function: SetAngleDeg
		//Purpose:  Sets the pulses for the robot's motors' encoders
		//Accepts:	An integer from 0 to 180
		//Returns:	Nothing
		//Throws:	
		//------------------------------------------------------------------------------------
		public string SetAngleDeg(int degAngle)
		{
			int pulses;
			string strHexPulses = "";

			//Set data in the range [0,360]
			while(degAngle > 360)
				degAngle =- 360;

			while(degAngle < 0)
				degAngle =+ 360;

			//
			pulses = (int)(11.25833*degAngle);

			strHexPulses = pulses.ToString("x4");

			sPort.WriteString("A003" + strHexPulses.Substring(2,2) + "A004" + strHexPulses.Substring(0,2));
			Thread.Sleep(50);
			sPort.WriteString("0A03" + strHexPulses.Substring(2,2) + "0A04" + strHexPulses.Substring(0,2));

			return (pulses.ToString() + strHexPulses);
		}

		//------------------------------------------------------------------------------------
		//Function: SetAngleRad
		//Purpose:  Sets the pulses for the robot's motors' encoders
		//Accepts:	A floating value from 0 to 3.56
		//Returns:	Nothing
		//Throws:	
		//------------------------------------------------------------------------------------
		public void SetAngleRad(double radAngle)
		{
			int pulses;
			string strHexPulses = "";
			
			while(radAngle > Math.PI*2)
				radAngle =- Math.PI*2;
			while(radAngle < 0)
				radAngle =+ Math.PI*2;

			pulses = (int)(11.25833*radAngle*(180/Math.PI));

			sPort.WriteString("A003" + strHexPulses.Substring(2,2) + "A004" + strHexPulses.Substring(0,2));
			Thread.Sleep(50);
			sPort.WriteString("0A03" + strHexPulses.Substring(2,2) + "0A04" + strHexPulses.Substring(0,2));
		}
		

		//------------------------------------------------------------------------------------
		//Function: SetEncoderHex
		//Purpose:  Sets the pulses for the robot's motors' encoders
		//Accepts:	A string the pulses with 4 hexadecimal digits  to include
		//Returns:	Nothing
		//Throws:	
		//------------------------------------------------------------------------------------
		public void SetEncoderHex(string hexString)
		{
			if(sPort.Opened)
			{
				hexString = hexString.Substring(0,4);
				sPort.WriteString("A003" + hexString.Substring(2,2) + "A004" + hexString.Substring(0,2));
				Thread.Sleep(50);
				sPort.WriteString("0A03" + hexString.Substring(2,2) + "0A04" + hexString.Substring(0,2));
			}
		}

		//------------------------------------------------------------------------------------
		//Function: GetSensorVal
		//Purpose:  Get the sensor value
		//Accepts:	A string with the sensor type and a value with the sensor number
		//Returns:	String sent through Serial Port
		//Throws:	
		//------------------------------------------------------------------------------------
		public string ReqSensorVal(string sensorType, int sensorNumber)
		{
			sensorType = sensorType.ToLower();
			string sensorCode = "";
			if(sensorNumber<256)		//Must be below 256, only 1 byte
			{
				switch(sensorType)
				{
					case "sonar":
						sensorCode = SONARREQ;
						break;
					case "ir":
						sensorCode = IRREQ;
						break;
					case "touch":
						sensorCode = TOUCHREQ;
						break;
					case "camera":
						sensorCode = CAMERAREQ;
						break;
					case "line":
						sensorCode = LINEREQ;
						break;
					case "compass":
						sensorCode = COMPASSREQ;
						break;
					case "laser":
						sensorCode = LASERREQ;
						break;
					default:
						break;
				}
				sensorCode = sensorCode + sensorNumber.ToString("x2") + "0000";
				this.sPort.WriteString(sensorCode);
				return sensorCode;
			}
			else
			{
				throw new Exception("Invalid sensor value\r\nValue must be lower than 256");
			}			
		}
		#endregion

	}
}