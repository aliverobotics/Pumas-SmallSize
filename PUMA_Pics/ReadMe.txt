SubGecko ver 1.21
					22:17 @ 2005/10/21
-A slight change was made to the PWM timer. Now the timer frequency is corretctly
 settled to a value close to 20KHz (~20.161KHz). With this, the motor speed has 
 a little boost over the older version. 

SubGecko ver 1.2
					20:38 @ 2005/10/21

-Flag.h was added, this header includes all the difinitions
 used in all the PICS and data that they share in common. 
 Making it easier this way to update the code.

-An interruption to the MV command was added. Whenever the robot
 is executing the MV command. If a BREAK_FLAG (0x0F) is recieved.
 The command is interrupted stoping both motors.

-Further detail was added to the comments all around the code.

-The funciton byte2Long was added to both slave PIC's in order
 to make the code cleaner.


NOTE:
-Whenever the master PIC is executing a command, data at serial port
 will be ignored, unless it is a BREAK_FLAG for the MV command.
 The simple protocol between the PPC and the robot is as follows:
	1) PPC sends a command
	2) Robot recieves and sends back an echo. Confirming it was recieved
	   correctly.
	4) Robot parses the command. If it is executable executes, otherwise,
	   sends back an error message.
	3) Once command is executed, aborted or an error ocurred,
	   it returns data if requested and an echo of original command recieved.
	5) PPC is available to send another command.