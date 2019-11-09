#define F_CPU 8000000 //8MHz clock


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>  

#include "SPI.h"
#include "LIS3DH.h"

  //led 1 pin 31 = PC6
  //led 2 pin	 = PC7
#define LED0 PC6
#define LED1 PC7 

#define C_BUTTON PD2 
  //to set input with pullup, DDRX = 0, PORTX = 1
  
  
#define ACC_SEL PD4


//PD4567

  
  
  //
  
  
 volatile uint8_t freeFallInt = 0; 
  
int main(void)
{
	DDRC |= 0xFF; //port c is outputs
	
	DDRD = 0x00; //all inputs
	PORTD |= ( 1<< C_BUTTON);//enable pullup
	
	//SPI lines are outputs and high
	DDRD |= (1<<DD4) | (1<<DD5) | (1<<DD6) | (1 <<DD7);
	PORTD |=(1<<PD4) | (1<<PD5) | (1<<PD6) | (1 <<PD7);
	
	SPI spi;
	
	//interrupts:
	cli(); //global interrupt off while changing this
	//set pins as input
	//PD0 > freefall
	DDRD &= ~(1<<INT0);
	//enable interrupt
	EIMSK |= 1; //enable INT0
	//set edge level
	EICRA |= 0x3; //rising edge on INT0
	EIFR |= 0x1; //clear the flag

	
	
	LIS3DH lis(spi,PORTD,ACC_SEL); 
	if(!lis.init())
		return -1;
	
	lis.setDataRange(LIS3DH_RANGE_8_G);
	lis.setDataRate(LIS3DH_DATARATE_200_HZ);
	lis.enableFreeFallInt1();
	sei(); //enable global interrupts
	while(1)
	{
		/*
		if(PIND & (1<<C_BUTTON))
		{
			PORTC |= ( 1 << LED1);
		}
		else
		{
			PORTC &= ~( 1 << LED1);
		}
		*/
		
		
		if(freeFallInt)
		{
			PORTC |= ( 1 << LED1);
			freeFallInt = 0;
			_delay_ms(2000);
			PORTC &= ~( 1 << LED1);
				
			lis.clearInterrupts();
		}
		float x = 0, y=0, z=0;
		lis.read(x,y,z);
		
		if(z>0)
		{
			PORTC |= ( 1 << LED0 );
		}
		else
		{
			PORTC &= ~( 1 << LED0 );
		}
		_delay_ms(1);
	}
};

ISR(INT0_vect) //handle interrupt on INT0
{
	freeFallInt = 1;

}


