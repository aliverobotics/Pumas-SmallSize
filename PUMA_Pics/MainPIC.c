/***********************************************************************
							RoboCup U.N.A.M
							
File Name:	MainPIC.c
Description:	This is the program loaded into the main PIC
					of the robots for the Robocup Project. It read from 
					serial port, parses and tells the motors' PICs 
					what to do.
Author:			Carlos Munive
Modified by:	Salvador Medina
Location:		Lab. Biorobotica
					Facultad de Ingenieria, U.N.A.M.
					Mexico
***********************************************************************/
#include <16F874a.h>			//Always check 
#include <stdlib.h>
#include <Flag.h>					//Our state definitions

#fuses HS,NOPROTECT
#use delay(clock=20000000)
#use rs232(baud=19200, xmit=PIN_C6, rcv=PIN_C7)	
#org 0x0F00, 0x0FFF void loader16F873(void) {} //protect bootloader code for the 4k 16F873/4

#include <input.c>

#define EEPROM_SDA  PIN_B1
#define EEPROM_SCL  PIN_B0
#use i2c(master,sda=EEPROM_SDA, scl=EEPROM_SCL)

#define PRINT_ERROR printf("error")	//Error Print macro
byte datos[10];									//Serial port buffer
int bufferPos;										//Actual Serial Port Buffer ptr
byte executing;									//Executing flag									
byte mainState;									//Main PIC state flag

void init_ext_eeprom() {					//Setup i2c comm pins
   output_float(eeprom_scl);
   output_float(eeprom_sda);
}

void ClearBuffer(){							//Clears out the serial port buffer
int i;	
	for(i = 0; i < STD_CMD_LENGTH; i++)
		{datos[i] = 0x00;}
}

void Writei2c(byte slaveID, byte address, byte data){	//Writes into the i2c comm port indicating slave ID, address and data to be written
	i2c_start();							//In our i2c implemented protocol, to write at a certain address in slave PIC's the order is:  slaveID>address>data
	i2c_write(slaveID);
	i2c_write(address);
	i2c_write(data);
	i2c_stop();
	delay_ms(1);
}

void WriteLeft(byte address, byte data){						//Writes a data into the specified address in the left motor PIC
	Writei2c(LEFT_MOTOR, address, data);
}

void WriteRight(byte address, byte data){					//Writes a data into the specified address in the right motor PIC
	Writei2c(RIGHT_MOTOR, address, data);
}

byte Readi2c(byte motor, byte address){						//Reads a given address from a specific motor PIC
byte data;						//In our i2c implemented protocol, to read a certain address in slave PIC the order is:  slaveID>address>interrupt>mask>read
byte mask;
	mask = motor+1;		//writing mask must be the slave PIC ID + 1
   i2c_start();
   i2c_write(motor);//0xa0
   i2c_write(address);
   i2c_start();
   i2c_write(mask);//0xa1
   data=i2c_read(0);
   i2c_stop();
   delay_ms(1);
   return(data);
}

byte LerMotIzq(byte address){									//Reads a given address from left motor PIC
   byte data;
   data = Readi2c(LEFT_MOTOR, address);
   return data;
}

byte LerMotDer(byte address){									//Reads a given address from right motor PIC
   byte data;
   data = Readi2c(RIGHT_MOTOR, address);
   return data;
}

void SetSpeedDir(byte motor, byte dir, byte pwmH, byte pwmL){	//Sets the PWM duty cycle of a given motor; range [0 ~ 0x1FF]
	Writei2c(motor, 0x01, pwmL);
	Writei2c(motor, 0x02, pwmH);
	Writei2c(motor, 0x00, dir);
}

void StopMotor(byte motor, int delay){							//Sets a lil brake by turning wheels in opposite direction from which they were turning for a preset time[ms]
	byte data;

	data = Readi2c(motor, 0x00);
	switch(data){
		case 1:
			SetSpeedDir(motor, 0x02, 0x01, 0xFF);
			break;
		case 2:
			SetSpeedDir(motor, 0x01, 0x01, 0xFF);
			break;
	}
	delay_ms(delay);
	SetSpeedDir(motor, 0x00, 0x00, 0x00);
}

void MotorDisparador(byte DIR) {									//Activates Shooter motor   [ 0: OFF; 1: ON ]
	switch(DIR){
		case 0 :// OFF
				output_low(PIN_C1);
				output_low(PIN_C2);
			   break;
		case 1 :// ON
				output_high(PIN_C2);
				output_high(PIN_C1);
			   break;
		}
}

void GirarRobot(byte turnH, byte turnL){						//Macro func used to make the robot turn any given encoder pulses [0~0x7FFF]
	byte value1, value2, DIR;										//Resume: LoadEncoders > Set GO Flag > Move motors > Wait for encoders cycle > Stop Motor
	value1 = 0; value2 = 0;
	DIR = turnH & 0x80;												//msb sets the turning direction, if 0 turns to right, if 1 turns to left
	
	//Load Encoder Values
	WriteLeft( 0x03,turnL);
	WriteLeft( 0x04,turnH & 0x7F);								
	WriteRight( 0x03,turnL);
	WriteRight( 0x04,turnH & 0x7F);
	
	//Set GO Flag
	WriteLeft( 0x05,GO_FLAG);
	WriteRight( 0x05,GO_FLAG);
	
	//Start Engines
	if (DIR == 0x00){
		WriteLeft(0x00, 0x01);
		WriteRight(0x00, 0x02);
	}
	else {
		WriteLeft( 0x00, 0x02);
		WriteRight( 0x00, 0x01);
	}
	 while ( (value1 != DONE_FLAG || value2 != DONE_FLAG) && mainState != BREAK_FLAG){	//Reads as: If none of the motors is done or the mainState flag isn't BREAK, continue
		value1 = LerMotIzq(0x05);																								
		value2 = LerMotDer(0x05);
	}
	
	//Stop both motors as soon as one of them is done
	WriteLeft(0x00, 0x00);
	WriteRight(0x00, 0x00);
	WriteLeft(0x05, BREAK_FLAG);
	WriteRight(0x05, BREAK_FLAG);
}

void DesplazarRobot(byte advanceH, byte advanceL){		//Macro func used to make the robot advance any given encoder pulses [0~0xFFFF]
	byte value1, value2, DIR;											//Resume: LoadEncoders > Set GO Flag > Move motors > Wait for encoders cycle > Stop Motor 
	value1 = 0; value2 = 0;
	// Si DIR = 0 atras
	//Set Direction
	DIR = advanceH & 0x80;									//msb sets the advancing direction, if 0 goes backwards, if 1 goes forward
	//Load Encoder Values 
	WriteLeft(0x03,advanceL);
	WriteLeft(0x04,advanceH & 0x7F);
	WriteRight(0x03,advanceL);
	WriteRight(0x04,advanceH & 0x7F);
	//Set GO Flag
	WriteLeft( 0x05, GO_FLAG);
	WriteRight( 0x05, GO_FLAG);
	
	if (DIR == 0x00){
	WriteLeft(0x00,0x02);
	WriteRight(0x00,0x02);
	}
	else{
	WriteLeft(0x00,0x01);
	WriteRight(0x00,0x01);
	}
	
	while ( (value1 != DONE_FLAG || value2 != DONE_FLAG)&& mainState != BREAK_FLAG){
		value1 = LerMotIzq(0x05);
		value2 = LerMotDer(0x05);
	} 
	
	//Stop both motors
	WriteLeft(0x05, BREAK_FLAG);
	WriteRight(0x05, BREAK_FLAG);
	WriteLeft(0x00, 0x00);
	WriteRight(0x00, 0x00);
}

void MoverRobot (byte advH, byte advL, byte trnH, byte trnL)		//Macro function for a MV command (turn and advance)
{													
	if( trnH != 0x00 || trnL != 0x00 ){					//Check if we need to turn				
		GirarRobot(trnH, trnL);
	}
	if( (advH != 0x00 || advL != 0x00) && mainState != BREAK_FLAG){					//Check if we need to advance
		DesplazarRobot(advH, advL);
	}
}

void ParseCommand(byte param0, byte param1, byte param2, byte param3, byte param4){		//Parses incoming message and executes if exists. If it doesn't exist, sends an error message
	byte data;
	
	switch(param0){
		case 0xA0:	//Stops a motor action (motor, delay)
					StopMotor(param1, 30);
					break;
		case 0xA1:	//Write data into motor pics (motor, address, data)
					Writei2c(param1, param2, param3);
					break;
		case 0xA2:	//Read  data from a slave  (motor, addr)
					data = Readi2c(param1, param2);
					printf("%x %x %x", param1, param2, data );
					break;
		case 0xA3 :	// MV - Turn and advance
					MoverRobot (param1, param2, param3, param4); 
				   	break;
		case 0x5D:	//Set Speed&Direction
					SetSpeedDir(param1, param2, param3, param4);
					break;
		case 0xFF : //Shooter	lsb:  0 - OFF; 1 - ON
					MotorDisparador(param4);
				   	break;
		default:	//Command was not found, print error thru SPort
					//printf("bufferPos = %x\r\n", bufferPos);
					PRINT_ERROR; delay_ms(1);
		} /* Fin del swtch() */
}

#int_rda
void serial_isr() {							//Serial Port Interruption procedure. Gathers info from serial port while we are not executing another command.
	byte temp;
	temp = gethex();
	if(!executing){
		datos[bufferPos] = temp;
		bufferPos++;
	}
	else{											//While we are executing if a BREAK comes into the port, the MV is interrupted
		if(temp == BREAK_FLAG){
			mainState = BREAK_FLAG; 
		}
	}
}

main() {
	int i;
	byte doneCommand[STD_CMD_LENGTH];
	//Initialization of our environment
	for(i = 0; i < STD_CMD_LENGTH; i++)
		{doneCommand[i] = 0x00;}
	executing = FALSE;						//Not executing
	mainState = GO_FLAG;
	bufferPos = 0;								//bufferPos at the start position
	ClearBuffer();									//Clear out buffer
	MotorDisparador(0);						//Start with shooter off
	Writei2c(LEFT_MOTOR, 0, 0);		//Sets the left motor static
	Writei2c(RIGHT_MOTOR, 0, 0);		//Sets the right motor static
	
	enable_interrupts(int_rda);				//Enable serial port interruption
	enable_interrupts(global);				//Enable all the interruptions
	
	setup_port_a( ALL_ANALOG );		//Port A is used for sensors
	setup_adc( ADC_CLOCK_INTERNAL );

	while (TRUE) {
		mainState = GO_FLAG;
		if(bufferPos == STD_CMD_LENGTH){		//if the command lenght is completed, we are ready to execute
			executing = TRUE;							//block serial port input to the buffer
				ParseCommand(datos[0], datos[1], datos[2], datos[3], datos[4]);	//Parse and execute the recieved command
				for(i = 0; i < STD_CMD_LENGTH; i++)
					{ doneCommand[i] = datos[i];}
				if(mainState == BREAK_FLAG)
					{ mainState = GO_FLAG; }
				bufferPos = 0;			//Reset serial port buffer
				ClearBuffer();
			executing = FALSE;		//unblock the serial port input
			printf("%x%x%x%x%x", doneCommand[0], doneCommand[1], doneCommand[2], doneCommand[3], doneCommand[4]);
		}
	}
} /* Fin del main() */
