/***********************************************************************
							RoboCup U.N.A.M
							
File Name:	RightMotor.c
Description:	This is the program loaded into the right motor's
					PIC of the robots for the Robocup Project. It 
					activates the motor, sets PWM duty cycle and
					makes a otor's encoder count.
Author:			Carlos Munive
Modified by:	Salvador Medina
Location:		Lab. Biorobotica
					Facultad de Ingenieria, U.N.A.M.
					Mexico
***********************************************************************/
#include <16F874A.h>
#include <Flag.h>
#fuses HS,NOWDT,NOPROTECT
#use delay(clock=20000000)
#use rs232(baud=19200, xmit=PIN_C6, rcv=PIN_C7)
#org 0x0F00, 0x0FFF void loader16F873(void) {}   //for the 4k 16F873/4

//Slave definition
#use i2c(SLAVE, SDA=PIN_C4, SCL=PIN_C3, address=RIGHT_MOTOR)

I2C_STATE fState;
byte address, buffer[0x10];			//Used as buffer for main PIC to write into it, with this buffer micro realizes action in our main cycle

#INT_SSP 
void ssp_interupt ()							//Communicatio State Machine
{														//	NOTHING -[slave addr]> CONTROL_READ -[addr]> ADDRESS_READ -- [data] (writes data in buffer) ----- > NOTHING
   byte incoming;								//																  										  \ 											  /	
														//																											\[interrupt](send data to buffer) /	
   if (i2c_poll() == FALSE) {					
      if (fState == ADDRESS_READ) {	//i2c_poll() returns false on interupt if there is no data on buffer
         i2c_write (buffer[address]);			//receiving the second random byte (mask) for read operation
         fState = NOTHING;           		
      }
   }
   else {
      incoming = i2c_read();							//There's something in the buffer, so we get it
      if (fState == NOTHING){						//if current state is NOTHING witch to CONTROL_READ state, besides first data is current PIC i2c address (useless)
         fState = CONTROL_READ;
      }
      else if (fState == CONTROL_READ) {	//If current state is CONTROL_READ  switch to ADDRESS_READ state and incoming data is address
         address = incoming;
         fState = ADDRESS_READ;
      }
      else if (fState == ADDRESS_READ) {	//If current state is ADDRESS_READ switch to NOTHING state and write the incoming data is the specified address
         buffer[address] = incoming;
         fState = NOTHING;
      }
   }
}

void Motor(byte DIR) {			//Check motor's manual for further details
	switch(DIR)
		{
		case 0x00 :// Deactivate
				output_low(PIN_C1);
				output_low(PIN_C0);
			   break;
	   case 0x01 :// Turns Counter-Clockwise (forward)
				output_low(PIN_C0);
				output_high(PIN_C1);
			   break;
		case 0x02 :// Turns Clockwise (backward)
				output_high(PIN_C0);
				output_low(PIN_C1);
			   break;
		case 0x03 :// Soft Break
				output_high(PIN_C1);
				output_high(PIN_C0);
			   break;
		}
}

byte resync() {			//Returns the lsb of the motor encoders
	byte seqind;
	seqind = input_a() & 1;		//get lsb state
	return seqind;
}

long byte2Long(byte byteL, byte byteH){		//Converts 2 bytes into a long [16 bits]
	long temp;
	temp =	byteH << 8;
	return (temp | byteL);
}

void main ()
{
	byte seq_index;
	int i;
	long actPWMDuty, k, pulsos, encoder;
	
	//Variable Initialization
	actPWMDuty = 0;
	pulsos = 0;
	encoder = 0;
	fState = NOTHING;
	address = 0x00;
	for ( i = 0; i < 0x10; i++)
		{buffer[i] = 0x00;}
		
		//Enable interrupts
		enable_interrupts(GLOBAL);
		enable_interrupts(INT_SSP);

 		setup_ccp1(CCP_PWM);   
 		setup_timer_2(T2_DIV_BY_16, 62, 1);			//Sets PWM frequency to 20KHz >> cycleTime = 1*62*( 16* (1/20000000) ) = 49.6us >> Freq = 1/cycleTime = 20.161KHz ~ 20KHz

		//Buffer array determines the motor state
		buffer[0] = 0x00; 		/* Direction */
		buffer[1] = 0xff; 			/* PWM low byte */
		buffer[2] = 0x01; 		/* PWM high byte */
		buffer[3] = 0x00; 		/* Encoder low byte */
		buffer[4] = 0x00; 		/* Encoder high byte */
		buffer[5] = 0x00; 		/* Encoders state flag*/

		actPWMDuty = byte2Long(buffer[1], buffer[2]);
 		set_pwm1_duty( actPWMDuty );
		Motor(0);


    while (TRUE) {
			actPWMDuty = byte2Long(buffer[1], buffer[2]);
 			set_pwm1_duty( actPWMDuty );
			Motor(buffer[0]);
			delay_ms(1);
			//If a GO signal is given encoder count
			if( buffer[5] == GO_FLAG )
			{
					encoder = byte2Long(buffer[3], buffer[4]);
					for(k = 0 ; k < encoder; k++)	// Modificada para stop
					{
						seq_index = resync();					//get encoder current state
			    		while((input_a() & 1) == seq_index)	//for next count wait till that state changes
			    		{
				    		Motor(buffer[0]);							//in the meantime we are able to change direction, this works to start the motor from 0 with encoder count
				    		if(buffer[5] == BREAK_FLAG)					//Used to get out of cycle with BREAK_FLAG
								{ break;}
						}
			    		actPWMDuty = byte2Long(buffer[1], buffer[2]);
 						set_pwm1_duty( actPWMDuty );
			    		if(buffer[5] == BREAK_FLAG)						//Used to get out of cycle with BREAK_FLAG
							{ break;}
					}
					buffer[3] = 0x00;								//Reset Encoder Count
					buffer[4] = 0x00;
					buffer[5] = DONE_FLAG;								//Set flag DONE_FLAG
			}
	} /* fin del while(true) */
} /* fin main() */